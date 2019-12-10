#include <vn/string_utils.hpp>

namespace vn {
bool
ends_with(std::string_view string, std::string_view ending)
{
  return string.size() >= ending.size() &&
    string.substr(string.size() - ending.size()).compare(ending) == 0;
}
} // namespace vn
