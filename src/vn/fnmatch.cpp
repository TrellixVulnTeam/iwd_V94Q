#include <vn/fnmatch.hpp>

#include <vn/string_utils.hpp>

namespace vn {

namespace {

std::string
translate_fnmatch_expr(std::string_view pattern)
{
  std::string result;
  result.reserve(pattern.size() * 2u);
  for (std::size_t i = 0u; i < pattern.length(); ++i) {
    const auto character = pattern[i];
    switch (character) {
      case '*': {
        result.append(".*");
        break;
      }
      case '?': {
        result.append(".?");
        break;
      }
      case '[': {
        std::size_t j = i + 1u;
        if (j < pattern.length() && pattern[j] == '!') {
          j += 1;
        }
        if (j < pattern.length() && pattern[j] == ']') {
          j += 1;
        }

        if (j = pattern.find(']', j); j != std::string_view::npos) {
          auto stuff = vn::replace_all(pattern.substr(i + 1, j), "\\", "\\\\");
          i = j + 1;

          if (stuff[0] == '!') {
            stuff[0] = '^';
          } else if (stuff[0] == '^') {
            stuff = "\\" + stuff;
          }
          result.append("[" + stuff + "]");
        } else {
          result.append("\\[");
        }

        break;
      }
      default: {
        result.append(vn::regex_escape(std::string_view(&character, 1)));
        break;
      }
    }
  }
  result.shrink_to_fit();
  return result;
}
} // namespace

fnmatch::fnmatch(std::string_view pattern)
  : _pattern(translate_fnmatch_expr(pattern))
{}

bool
fnmatch::operator()(const std::filesystem::path& file_path) const
{
  std::cmatch match;
  return std::regex_match(file_path.c_str(), match, _pattern);
}

} // namespace vn
