#include "iwd/check_call.hpp"

#include <boost/process.hpp>
#include <boost/process/search_path.hpp>
#include <chrono>
#include <iomanip>
#include <vn/string_utils.hpp>

namespace iwd {

namespace {
boost::filesystem::path
to_boost_path(const std::filesystem::path& path)
{
  return boost::filesystem::path(path.string());
}
} // namespace

namespace bp = boost::process;

void
check_call(
  const std::string& app_name,
  std::vector<std::string> proc_args,
  const check_call_arguments& args)
{
  const auto executable_path = bp::search_path(app_name);
  if (executable_path.empty()) {
    throw std::runtime_error(
      vn::make_message("Could not resolve path to", app_name, " executable"));
  }
  const auto working_directory = to_boost_path(
    args.working_directory.value_or(vn::directory::current()).path());

  bp::child proc(
    executable_path,
    bp::args(proc_args),
    bp::start_dir(working_directory),
    bp::std_out > stdout,
    bp::std_err > stderr,
    bp::std_in < stdin);

  if (args.timeout) {
    if (!proc.wait_for(args.timeout.value())) {
      throw std::runtime_error(
        vn::make_message("Timeout while calling", std::quoted(app_name)));
    }
  } else {
    proc.wait();
  }

  if (proc.exit_code()) {
    throw std::runtime_error(vn::make_message(
      "Process",
      std::quoted(app_name),
      "exited with status",
      proc.exit_code()));
  }
}

} // namespace iwd
