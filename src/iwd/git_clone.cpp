#include "iwd/git_clone.hpp"

#include "iwd/check_call.hpp"
#include <vector>

namespace iwd {

void
git_clone(
  const std::string& url,
  const std::filesystem::path& target_directory,
  const std::string& branch)
{
  std::string executable = "git";
  std::vector<std::string> args = {
    "clone",    "--depth", "1", "--recursive",
    "--branch", branch,    url, target_directory.string(),
  };
  if (!std::filesystem::is_directory(target_directory)) {
    check_call(executable, args);
  }
}

} // namespace iwd
