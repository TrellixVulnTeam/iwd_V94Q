#ifndef IWD__DIRECTORIES_HPP
#define IWD__DIRECTORIES_HPP

#include <string>
#include <vn/directory.hpp>

namespace iwd {

struct directories
{
  directories(const std::filesystem::path& working_directory);

  directories(const directories&) = default;
  directories& operator=(const directories&) = default;

  directories(directories&&) = default;
  directories& operator=(directories&&) = default;

  ~directories() = default;

  const vn::directory& working_directory() const noexcept;
  const vn::directory& cache_directory() const noexcept;
  const vn::directory& build_directory() const noexcept;
  const vn::directory& source_directory() const noexcept;
  const vn::directory& install_directory() const noexcept;

  vn::directory make_build_directory(const std::string& build_id) const;

private:
  vn::directory _working_directory;
  vn::directory _cache_directory;
  vn::directory _build_directory;
  vn::directory _source_directory;
  vn::directory _install_directory;
};

} // namespace iwd

#endif
