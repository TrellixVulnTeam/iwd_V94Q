#ifndef IWD__EXECUTABLE_HPP
#define IWD__EXECUTABLE_HPP

#include <filesystem>
#include <iwd/subprocess.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <vn/directory.hpp>

namespace iwd {
struct executable
{
  explicit executable(const std::filesystem::path& path);
  void run(
    const std::vector<std::string>& proc_args = {},
    const subprocess_arguments& args = {}) const;

private:
  std::filesystem::path _executable_path;
};

std::optional<executable>
try_find_executable(
  const std::string& name,
  const std::optional<std::vector<std::filesystem::path>>& extra_search_paths =
    std::nullopt);

executable
find_executable_or_throw(
  const std::string& name,
  const std::optional<std::vector<std::filesystem::path>>& extra_search_paths =
    std::nullopt);

} // namespace iwd

#endif
