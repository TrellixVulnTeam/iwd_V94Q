#ifndef IWD__SUBPROCESS_HPP
#define IWD__SUBPROCESS_HPP

#include <chrono>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>
#include <vn/directory.hpp>

namespace iwd {

struct check_call_arguments
{
  std::optional<std::chrono::seconds> timeout;
  std::optional<vn::directory> working_directory;
};

void
check_call(
  const std::string& app_name,
  const std::vector<std::string>& proc_args,
  const check_call_arguments& args = {});

void
check_call(
  const std::filesystem::path& app_path,
  const std::vector<std::string>& proc_args,
  const check_call_arguments& args = {});

} // namespace iwd

#endif
