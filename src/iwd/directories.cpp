#include "iwd/directories.hpp"

#include <string_view>

namespace iwd {

namespace {
constexpr std::string_view cache_dir_name = "cache";
constexpr std::string_view build_dir_name = "build";
constexpr std::string_view source_dir_name = "source";
constexpr std::string_view install_dir_name = "install";

vn::directory
create_dir_in(const vn::directory& dir, const std::string_view& name)
{
  return vn::directory::create(dir.path() / name);
}

} // namespace

directories::directories(const std::filesystem::path& working_directory)
  : _working_directory(vn::directory::create(working_directory))
  , _cache_directory(create_dir_in(_working_directory, cache_dir_name))
  , _build_directory(create_dir_in(_working_directory, build_dir_name))
  , _source_directory(create_dir_in(_working_directory, source_dir_name))
  , _install_directory(create_dir_in(_working_directory, install_dir_name))
{}

const vn::directory&
directories::working_directory() const noexcept
{
  return _working_directory;
}

const vn::directory&
directories::cache_directory() const noexcept
{
  return _cache_directory;
}

const vn::directory&
directories::build_directory() const noexcept
{
  return _build_directory;
}

const vn::directory&
directories::source_directory() const noexcept
{
  return _source_directory;
}

const vn::directory&
directories::install_directory() const noexcept
{
  return _install_directory;
}

vn::directory
directories::make_build_directory(const std::string& build_id) const
{
  return create_dir_in(_build_directory, build_id);
}

} // namespace iwd
