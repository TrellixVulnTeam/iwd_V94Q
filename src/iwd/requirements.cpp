#include "iwd/requirements.hpp"

#include "download_file.hpp"
#include "git_clone.hpp"
#include "iwd/extract_tarfile.hpp"
#include "iwd/logging.hpp"
#include <vn/file.hpp>
#include <vn/string_utils.hpp>

namespace iwd {

namespace {
void
download_and_extract(
  const std::string& source,
  const std::filesystem::path& download_destination,
  const std::filesystem::path& extract_destination)
{
  if (!std::filesystem::exists(download_destination)) {
    info("Downloading {}", source);
    iwd::download_file(source, download_destination);
  }
  if (!std::filesystem::exists(extract_destination)) {
    info("Extracting {}", download_destination.string());
    iwd::extract_tarfile(
      download_destination, vn::directory::create(extract_destination));
  }
}

} // namespace

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
  , _source_directory(nullptr)
  , _requirement(req)
{}

void
requirement_handler::source()
{
  const auto namever = name_version(_requirement);
  const auto source_url = _requirement.get_url();
  const auto source_path = _directories.source_directory().path() / namever;
  const auto download_path = _directories.cache_directory().path() / namever;

  if (vn::ends_with(source_url, ".git")) {

    if (!std::filesystem::exists(source_path)) {
      info("Cloning {}", source_url);
      iwd::git_clone(source_url, source_path, _requirement.get_version());
    }
  } else if (vn::ends_with(source_url, "tar.gz")) {
    download_and_extract(source_url, download_path, source_path);

    // Most of the archives contain single root directory that contains the
    // actual content. If that is the case, make the _source_directory variable
    // point to this single directory instead of the calculated source location.
    // This makes setup way easier for the user.
    // TODO - Make it so, that if that is the case, the intermediate directory
    // is moved as a source
    auto source_directory = vn::directory(source_path);
    const auto file_count = std::distance(
      source_directory.list().begin(), source_directory.list().end());
    if (file_count == 1) {
      const auto first_item = source_directory.list().begin()->path();
      if (std::filesystem::is_directory(first_item)) {
        _source_directory = std::make_unique<vn::directory>(first_item);
      }
    }

  } else {
    throw std::runtime_error(vn::make_message(
      "Unsupported url",
      std::quoted(source_url),
      "must point to either tar.gz or git repository"));
  }
  if (!_source_directory) {
    _source_directory = std::make_unique<vn::directory>(source_path);
  }
}

} // namespace iwd
