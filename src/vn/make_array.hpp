#ifndef VN__MAKE_ARRAY_HPP
#define VN__MAKE_ARRAY_HPP

#include <array>
#include <utility>

namespace vn {
template<typename D, typename... Args>
constexpr std::array<D, sizeof...(Args)>
make_array(Args&&... args)
{
  return { std::forward<Args>(args)... };
}
} // namespace vn

#endif
