#include "iwd/quicktype/iwd.hpp"

#include "iwd/cmake_configuration.hpp"
#include "iwd/directories.hpp"
#include "iwd/domain.hpp"
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
    const iwd::domain& domain,
    const quicktype::Requirement& req);

  void source();
  void configure(const iwd::cmake_configuration& configuration);
  void patch();
  void install();

private:
  bool is_cmake_build() const noexcept;
  iwd::domain _domain;
  std::unique_ptr<vn::directory> _source_directory;
  std::unique_ptr<vn::directory> _build_directory;
  quicktype::Requirement _requirement;
};

} // namespace iwd
