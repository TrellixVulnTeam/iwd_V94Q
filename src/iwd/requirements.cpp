#include "iwd/requirements.hpp"

#include <vn/file.hpp>
#include <vn/string_utils.hpp>

namespace iwd {

std::string
name_version(const quicktype::Requirement& requirement)
{
  return vn::join("", requirement.get_name(), "-", requirement.get_version());
}

std::vector<quicktype::Requirement>
parse_requirements(const std::filesystem::path& iwd_config_path)
{
  quicktype::Iwd result =
    nlohmann::json::parse(vn::read_whole_file(iwd_config_path));
  // TODO - There is potentially huge copy of the resources here
  return result.get_requirements();
}

} // namespace iwd
