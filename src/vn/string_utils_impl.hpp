#ifndef VN__STRING_UTILS_IMPL_HPP
#define VN__STRING_UTILS_IMPL_HPP

#include <sstream>
#include <string_view>

namespace vn {
namespace detail {
struct join_writer
{
  join_writer(const std::string_view& base)
    : _base(base)
  {}

  template<typename Arg>
  void write_first(Arg&& arg)
  {
    _stream << arg;
  }

  template<typename Arg>
  void write(Arg&& arg)
  {
    _stream << _base << arg;
  }

  std::string result() const { return _stream.str(); }

private:
  std::string_view _base;
  std::stringstream _stream;
};

template<typename First, typename... Rest>
void
join_impl(join_writer& writer, const First& first, const Rest&... rest)
{
  writer.write_first(first);
  (writer.write(rest), ...);
}

template<typename T>
std::basic_string<T>
replace_all(
  std::basic_string_view<T> input,
  std::basic_string_view<T> occurrences,
  std::basic_string_view<T> replacement)
{
  using string_t = std::basic_string<T>;
  using string_view_t = std::basic_string_view<T>;
  using size_type = typename string_view_t::size_type;

  string_t result(input);
  size_type index = result.find(occurrences, 0u);
  for (; index != string_t::npos; index = result.find(occurrences, index)) {
    result.replace(index, occurrences.length(), replacement);
    index += replacement.length();
  }
  return result;
}

} // namespace detail

template<typename... Args>
std::string
join(const std::string& base, const Args&... args)
{
  detail::join_writer writer(base);
  detail::join_impl(writer, args...);
  return writer.result();
}

template<typename Iterator>
std::string
join_range(const std::string& base, vn::iterator_range<Iterator> range)
{
  std::stringstream ss;
  if (std::distance(range.begin(), range.end()) == 0u) {
    return {};
  }
  ss << *(range.begin());
  for (const auto& el :
       make_iterator_range(std::next(range.begin()), range.end())) {
    ss << base << el;
  }
  return ss.str();
}

template<typename... Args>
std::string
make_message(const Args&... args)
{
  return join(" ", args...);
}

} // namespace vn

#endif
