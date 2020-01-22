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

std::string
replace_all(
  std::string_view input,
  std::string_view occurrences,
  std::string_view replacement)
{
  return detail::replace_all<std::string::value_type>(
    input, occurrences, replacement);
}

std::wstring
replace_all(
  std::wstring_view input,
  std::wstring_view occurrences,
  std::wstring_view replacement)
{
  return detail::replace_all<std::wstring::value_type>(
    input, occurrences, replacement);
}

} // namespace vn
