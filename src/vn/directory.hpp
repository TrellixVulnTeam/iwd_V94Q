#ifndef VN__DIRECTORY_HPP
#define VN__DIRECTORY_HPP

#include <filesystem>
#include <vn/iterator_range.hpp>

namespace vn {

struct directory
{
  explicit directory(std::filesystem::path file_path);

  static directory current();
  static directory create(const std::filesystem::path& path);

  vn::iterator_range<std::filesystem::recursive_directory_iterator> walk();
  vn::iterator_range<std::filesystem::directory_iterator> list();

  directory(const directory&) = default;
  directory& operator=(const directory&) = default;

  directory(directory&&) = default;
  directory& operator=(directory&&) = default;

  ~directory() = default;

  const std::filesystem::path& path() const noexcept;

private:
  std::filesystem::path _path;
};

directory
create_directories(std::filesystem::path& directory_path);

} // namespace vn

#endif
