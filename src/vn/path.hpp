#ifndef VN__PATH_HPP
#define VN__PATH_HPP

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace vn {

std::pair<
  std::optional<std::filesystem::path>,
  std::optional<std::filesystem::path>>
split_path(const std::filesystem::path& path);

}

#endif
