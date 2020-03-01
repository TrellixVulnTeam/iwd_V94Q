#ifndef VN__FILE_HPP
#define VN__FILE_HPP

#include <filesystem>
#include <optional>
#include <string>

namespace vn {

std::string
read_whole_file(const std::filesystem::path& file_path);

std::optional<std::string>
read_whole_file(
  const std::filesystem::path& file_path,
  std::error_code& ec) noexcept;

void
create_text_file(
  const std::filesystem::path& file_path,
  std::string_view contents);

} // namespace vn

#endif
