#include <iwd/patch.hpp>

#include <fstream>
#include <vn/file.hpp>

namespace iwd {

void
replace_in_file(
  const std::filesystem::path& file_path,
  std::string_view occurence,
  std::string_view replacement)
{
  std::fstream fs(file_path.string())
}

} // namespace iwd
