#ifndef IWD__LOGGING_HPP
#define IWD__LOGGING_HPP

#include <spdlog/spdlog.h>

namespace iwd {
template<typename... Args>
void
info(std::string_view fmt, Args&&... args)
{
  spdlog::info(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void
error(std::string_view fmt, Args&&... args)
{
  spdlog::error(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void
warning(std::string_view fmt, Args&&... args)
{
  spdlog::warn(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void
debug(std::string_view fmt, Args&&... args)
{
  spdlog::debug(fmt, std::forward<Args>(args)...);
}

} // namespace iwd

#endif
