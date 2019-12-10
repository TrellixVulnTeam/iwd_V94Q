#ifndef VN__MAKE_STRING_HPP
#define VN__MAKE_STRING_HPP

#include <string>
#include <string_view>

namespace vn {

template<typename... Args>
std::string
join(const std::string& base, const Args&... args);

template<typename... Args>
std::string
make_message(const Args&... args);

bool
ends_with(std::string_view string, std::string_view ending);

bool
starts_with(std::string_view string, std::string_view beggining);

} // namespace vn

#include <vn/string_utils_impl.hpp>

#endif
