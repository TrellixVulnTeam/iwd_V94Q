#include <vn/error.hpp>

namespace vn {

std::error_code
errno_as_error_code() noexcept
{
  return std::error_code(errno, std::system_category());
}

} // namespace vn
