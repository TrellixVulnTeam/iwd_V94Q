#include <vn/string_utils.hpp>

namespace vn {
bool
ends_with(std::string_view string, std::string_view ending)
{
  return string.size() >= ending.size() &&
    string.substr(string.size() - ending.size()).compare(ending) == 0;
}

bool
starts_with(std::string_view string, std::string_view beggining)
{
  return string.size() >= beggining.size() &&
    string.substr(0u, beggining.size() + 1u).compare(beggining) == 0;
}

} // namespace vn
