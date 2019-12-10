#ifndef IWD__GIT_CLONE_HPP
#define IWD__GIT_CLONE_HPP

#include <filesystem>
#include <string>

namespace iwd {

void
git_clone(
  const std::string& url,
  const std::filesystem::path& target_directory,
  const std::string& branch);
}

#endif
