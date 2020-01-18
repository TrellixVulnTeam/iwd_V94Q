#ifndef VN__FNMATCH_HPP
#define VN__FNMATCH_HPP

#include <filesystem>
#include <regex>
#include <string>
#include <string_view>

namespace vn {
namespace detail {

std::string
translate_fnmatch_expr(std::string_view pattern);

}

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
