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

std::string
regex_escape(std::string_view text)
{
  std::string result;
  // Reserve size for the worst case scenario - each character needs to be
  // escaped
  result.reserve(text.size() * 2u);
  for (const auto character : text) {
    switch (character) {
      case '(':
      case ')':
      case '[':
      case ']':
      case '{':
      case '}':
      case '?':
      case '*':
      case '+':
      case '-':
      case '|':
      case '^':
      case '$':
      case '\\':
      case '.':
      case '\t':
      case '\n':
      case '\r':
      case '\v':
      case '\f': {
        result.push_back('\\');
        result.push_back(character);
        break;
      }
      default: {
        result.push_back(character);
        break;
      }
    }
  }
  result.shrink_to_fit();
  return result;
}

} // namespace vn
