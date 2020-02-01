#include <vn/file.hpp>

#include <fstream>
#include <streambuf>

namespace vn {

std::string
read_whole_file(const std::filesystem::path& file_path)
{
  std::FILE* fp = std::fopen(file_path.c_str(), "rb");
  if (fp) {
    std::string contents;
    std::fseek(fp, 0, SEEK_END);
    contents.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    return contents;
  }
  throw std::system_error(std::error_code(errno, std::system_category()));
}

void
create_text_file(
  const std::filesystem::path& file_path,
  std::string_view contents)
{
  std::FILE* fp = std::fopen(file_path.c_str(), "w");
  if (fp) {
    const auto bytes_written =
      std::fwrite(contents.data(), sizeof(char), contents.size(), fp);
    std::fclose(fp);
    return;
  }
  throw std::system_error(std::error_code(errno, std::system_category()));
}

} // namespace vn
