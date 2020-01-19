#ifndef VN__STRING_TEMPLATE_HPP
#define VN__STRING_TEMPLATE_HPP

#include <functional>
#include <memory>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace vn {

//// Simple text templating engine. Renders provided string with replaced
/// occurences of $(VARIABLE_NAME).
struct string_template
{
  using value_provider_functor =
    std::function<std::optional<std::string>(const std::string&)>;

  /// Creates string template with target string as a template. Throws on
  /// empty string
  string_template(std::string_view string);

  /// Replaces all occurences of $(VARIABLE_NAME) in template with values
  /// retrieved from functor and returns rendered string. Throws
  /// `invalid_argument` when variable is specified in template string, and is
  /// not provided by the functor.
  [[nodiscard]] std::string render(value_provider_functor value_provider);

  /// Overload of render with functor that accepts plain map and converts it
  /// to satisfy functor requirements.
  [[nodiscard]] std::string render(
    const std::map<std::string, std::string>& value_map);

private:
  // TODO - This is a struct, so the caching functionality can be provided in
  // the future without breaking compatibility.
  std::string _template;
};

/// Creates internally string_template object, and calls render on it.
[[nodiscard]] std::string
render_template(
  std::string_view string_template,
  string_template::value_provider_functor functor);

} // namespace vn

#endif
