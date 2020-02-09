#ifndef VN__STRING_CONVERSIONS_HPP
#define VN__STRING_CONVERSIONS_HPP

#include <charconv>
#include <optional>
#include <string_view>

namespace vn {

template<typename T>
std::optional<T>
try_convert(std::string_view text);

}

#include <vn/string_conversions_impl.hpp>

#endif
