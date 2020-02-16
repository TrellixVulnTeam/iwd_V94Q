#include <iwd/executable.hpp>

#include <boost/process.hpp>
#include <boost/process/search_path.hpp>
#include <vn/string_utils.hpp>

namespace iwd {

namespace bp = boost::process;

executable::executable(const std::filesystem::path& path)
  : _executable_path(path)
{}

void
executable::check_call(
  const std::vector<std::string>& proc_args,
  const subprocess_arguments& args) const
{
  iwd::check_call(_executable_path, proc_args, args);
}

std::optional<executable>
try_find_executable(
  const std::string& name,
  const std::optional<std::vector<std::filesystem::path>>& extra_search_paths)
{
  auto search_paths = boost::this_process::path();
  if (extra_search_paths) {
    std::transform(
      extra_search_paths->begin(),
      extra_search_paths->end(),
      std::back_inserter(search_paths),
      [](const std::filesystem::path& p) {
        return boost::filesystem::path(p.generic_string());
      });
  }

  auto proc = bp::search_path(boost::filesystem::path(name), search_paths);
  return proc.empty() ? std::optional<executable>()
                      : std::make_optional(executable(
                          std::filesystem::path(proc.generic_string())));
}

executable
find_executable_or_throw(
  const std::string& name,
  const std::optional<std::vector<std::filesystem::path>>& extra_search_paths)
{
  auto result = try_find_executable(name, extra_search_paths);
  if (!result) {
    throw std::runtime_error(
      vn::make_message("Could not find program named", name));
  }
  return result.value();
}

} // namespace iwd
