#ifndef IWD__EXTRACT_TARFILE_HPP
#define IWD__EXTRACT_TARFILE_HPP

#include <filesystem>
#include <functional>
#include <optional>
#include <vn/directory.hpp>

namespace iwd {

struct tar_entry
{
  std::filesystem::path file_path;
};

using tar_entry_visitor_t = std::function<bool(tar_entry&)>;

void
extract_tarfile(
  const std::filesystem::path& tar_location,
  const vn::directory& extract_location,
  std::optional<tar_entry_visitor_t> entry_visitor = std::nullopt);

} // namespace iwd

#endif
