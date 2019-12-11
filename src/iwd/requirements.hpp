#include "iwd/quicktype/iwd.hpp"

#include "iwd/directories.hpp"
#include <filesystem>
#include <vector>

namespace iwd {

std::string
name_version(const quicktype::Requirement& requirement);

std::vector<quicktype::Requirement>
parse_requirements(const std::filesystem::path& iwd_config_path);

struct requirement_handler
{
  requirement_handler(
    const iwd::directories& directories,
    const quicktype::Requirement& req);

  void download();

private:
  iwd::directories _directories;
  quicktype::Requirement _requirement;
};

} // namespace iwd
