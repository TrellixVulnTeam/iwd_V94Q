#ifndef VN__ERROR_HPP
#define VN__ERROR_HPP

#include <system_error>

namespace vn {

std::error_code
errno_as_error_code() noexcept;

}

#endif
