#ifndef VN__FILE_HPP
#define VN__FILE_HPP

#include <filesystem>
#include <string>

namespace vn {

std::string
read_whole_file(const std::filesystem::path& file_path);

}

#endif
