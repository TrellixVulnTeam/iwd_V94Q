#include <cassert>
#include <vn/directory.hpp>

namespace vn {

directory::directory(std::filesystem::path file_path)
  : _path(file_path)
{
  assert(
    std::filesystem::is_directory(file_path) &&
    "Specified path does not point to the directory");
}

const std::filesystem::path&
directory::path() const noexcept
{
  return _path;
}

directory
directory::current()
{
  return directory(std::filesystem::current_path());
}

vn::iterator_range<std::filesystem::recursive_directory_iterator>
directory::walk()
{
  return make_iterator_range(
    std::filesystem::recursive_directory_iterator(path()),
    std::filesystem::recursive_directory_iterator());
}

vn::iterator_range<std::filesystem::directory_iterator>
directory::list()
{
  return make_iterator_range(
    std::filesystem::directory_iterator(path()),
    std::filesystem::directory_iterator());
}

directory
directory::create(const std::filesystem::path& directory_path)
{
  std::filesystem::create_directories(directory_path);
  return directory(directory_path);
}

} // namespace vn
