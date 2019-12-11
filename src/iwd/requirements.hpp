#include "iwd/quicktype/iwd.hpp"

#include <filesystem>
#include <vector>

namespace iwd {

std::string
name_version(const quicktype::Requirement& requirement);

std::vector<quicktype::Requirement>
parse_requirements(const std::filesystem::path& iwd_config_path);

} // namespace iwd
