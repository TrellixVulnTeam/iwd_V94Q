#ifndef VN__FNMATCH_HPP
#define VN__FNMATCH_HPP

#include <filesystem>
#include <regex>
#include <string>
#include <string_view>

namespace vn {
struct fnmatch
{
public:
  fnmatch(std::string_view pattern);
  bool operator()(const std::filesystem::path& file_path) const;

private:
  std::regex _pattern;
};

} // namespace vn

#endif
