#ifndef VN__MAKE_STRING_HPP
#define VN__MAKE_STRING_HPP

#include <string>

namespace vn {

template<typename... Args>
std::string
join(const std::string& base, const Args&... args);

template<typename... Args>
std::string
make_message(const Args&... args);

} // namespace vn

#include <vn/string_utils_impl.hpp>

#endif
