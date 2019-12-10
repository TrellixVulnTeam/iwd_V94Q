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

} // namespace detail

template<typename... Args>
std::string
join(const std::string& base, const Args&... args)
{
  detail::join_writer writer(base);
  detail::join_impl(writer, args...);
  return writer.result();
}

template<typename... Args>
std::string
make_message(const Args&... args)
{
  return join(" ", args...);
}

} // namespace vn

#endif
