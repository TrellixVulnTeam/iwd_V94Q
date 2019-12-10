#ifndef VN__FUNCTION_POINTER_HPP
#define VN__FUNCTION_POINTER_HPP

namespace vn {

template<typename F>
using function_pointer = std::add_pointer_t<F>;

}

#endif
