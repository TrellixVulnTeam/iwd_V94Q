#include <vn/string_template.hpp>

#include <iomanip>
#include <sstream>
#include <vn/regex.hpp>
#include <vn/string_utils.hpp>

namespace vn {

namespace {

const auto expression_regex = std::regex(R"__regex__(\$\((\w+)\))__regex__");

std::string
get_value_or_throw(
  string_template::value_provider_functor& functor,
  const std::string& key)
{
  auto value = std::invoke(functor, key);
  if (!value) {
    throw std::invalid_argument(vn::make_message(
      "Could not obtain value for", std::quoted(key), "argument"));
  }
  return *value;
}
} // namespace

string_template::string_template(std::string_view string)
  : _template(string)
{
  if (_template.empty()) {
    throw std::invalid_argument("Template string is empty");
  }
}

std::string
string_template::render(value_provider_functor functor)
{
  std::stringstream result;
  std::string_view view = _template;
  std::cmatch match;

  while (!view.empty() &&
         std::regex_search(view.begin(), view.end(), match, expression_regex)) {
    result << match.prefix().str();
    result << get_value_or_throw(functor, match[1].str());
    view = std::string_view(match.suffix().first, match.suffix().length());
  }
  if (!view.empty()) {
    result << view;
  }
  return result.str();
}

std::string
string_template::render(const std::map<std::string, std::string>& value_map)
{
  return render(
    [&value_map](const std::string& value) -> std::optional<std::string> {
      auto it = value_map.find(value);
      if (it == value_map.end()) {
        return std::nullopt;
      }
      return it->second;
    });
}

std::string
render_template(
  std::string_view string_template,
  string_template::value_provider_functor functor)
{
  vn::string_template str_template(string_template);
  return str_template.render(functor);
}

} // namespace vn
