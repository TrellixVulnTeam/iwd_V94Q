#include "iwd/requirements.hpp"

#include <vn/file.hpp>

namespace iwd {

std::vector<quicktype::Requirement>
parse_requirements(const std::filesystem::path& iwd_config_path)
{
  quicktype::Iwd result =
    nlohmann::json::parse(vn::read_whole_file(iwd_config_path));
  // TODO - There is potentially huge copy of the resources here
  return result.get_requirements();
}

} // namespace iwd
