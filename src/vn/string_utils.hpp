#ifndef VN__MAKE_STRING_HPP
#define VN__MAKE_STRING_HPP

#include <string>
#include <string_view>
#include <vn/iterator_range.hpp>

namespace vn {

namespace detail {
template<typename T>
std::basic_string<T>
replace_all(
  std::basic_string_view<T> input,
  std::basic_string_view<T> occurrences,
  std::basic_string_view<T> replacement);
}

template<typename... Args>
std::string
join(const std::string& base, const Args&... args);

template<typename Iterator>
std::string
join_range(const std::string& base, vn::iterator_range<Iterator> range);

template<typename... Args>
std::string
make_message(const Args&... args);

bool
ends_with(std::string_view string, std::string_view ending);

bool
starts_with(std::string_view string, std::string_view beggining);

std::string
regex_escape(std::string_view text);

std::string
replace_all(
  std::string_view input,
  std::string_view occurrences,
  std::string_view replacement);

std::wstring
replace_all(
  std::wstring_view input,
  std::wstring_view occurrences,
  std::wstring_view replacement);

} // namespace vn

#include <vn/string_utils_impl.hpp>

#endif
