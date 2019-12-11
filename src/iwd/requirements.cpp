#include "iwd/requirements.hpp"

#include "download_file.hpp"
#include "git_clone.hpp"
#include "iwd/logging.hpp"
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

requirement_handler::requirement_handler(
  const iwd::directories& directories,
  const quicktype::Requirement& req)
  : _directories(directories)
  , _requirement(req)
{}

void
requirement_handler::download()
{
  const auto namever = name_version(_requirement);
  const auto source_url = _requirement.get_url();
  const auto source_path = _directories.source_directory().path() / namever;
  const auto download_path = _directories.cache_directory().path() / namever;

  if (vn::ends_with(source_url, ".git")) {

    if (!std::filesystem::exists(source_path)) {
      info("Cloning {}", source_url);
      iwd::git_clone(source_url, source_path, _requirement.get_version());
    } else {
      info("Using cached version for {}", source_url);
    }

  } else if (vn::ends_with(source_url, "tar.gz")) {

    if (!std::filesystem::exists(download_path)) {
      info("Downloading {}", source_url);
      iwd::download_file(source_url, download_path);
    } else {
      info("Using cached version for {}", source_url);
    }

  } else {
    throw std::runtime_error(vn::make_message(
      "Unsupported url",
      std::quoted(source_url),
      "must point to either tar.gz or git repository"));
  }
}

} // namespace iwd
