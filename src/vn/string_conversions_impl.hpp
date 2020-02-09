#ifndef VN__STRING_CONVERSIONS_IMPL_HPP
#define VN__STRING_CONVERSIONS_IMPL_HPP

#include <charconv>
#include <optional>
#include <string_view>

namespace vn {

template<typename T>
std::optional<T>
try_convert(std::string_view text)
{
  T result = 0;
  if (auto [p, ec] = std::from_chars(text.begin(), text.end(), result);
      ec == std::errc()) {
    return result;
  }
  return std::nullopt;
}

} // namespace vn

#endif
