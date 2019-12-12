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

std::string
replace_all(
  std::string_view input,
  std::string_view occurrence,
  std::string_view replacement)
{
  std::string result(input);
  std::string::size_type index = index = result.find(occurrence, 0u);
  for (; index != std::string::npos; index = result.find(occurrence, index)) {
    result.replace(index, occurrence.length(), replacement);
    index += replacement.length();
  }
  return result;
}

} // namespace vn
