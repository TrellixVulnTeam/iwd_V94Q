#include <vn/path.hpp>

namespace vn {

std::pair<
  std::optional<std::filesystem::path>,
  std::optional<std::filesystem::path>>
split_path(const std::filesystem::path& path)
{
  return std::make_pair(
    path.has_parent_path() ? std::make_optional(path.parent_path())
                           : std::nullopt,
    path.has_filename() ? std::make_optional(path.filename()) : std::nullopt);
}

bool
lexists(const std::filesystem::path& p)
{
  return std::filesystem::exists(p) || std::filesystem::is_symlink(p);
}

} // namespace vn
