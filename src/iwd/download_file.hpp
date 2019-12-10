#ifndef IWD__DOWNLOAD_FILE_HPP
#define IWD__DOWNLOAD_FILE_HPP

#include <chrono>
#include <filesystem>
#include <optional>
#include <string>

namespace iwd {

struct download_settings
{
  std::optional<std::chrono::seconds> timeout;
  std::optional<std::size_t> port;
};

void
download_file(
  const std::string& url,
  const std::filesystem::path& output,
  const download_settings& settings = {});

} // namespace iwd

#endif
