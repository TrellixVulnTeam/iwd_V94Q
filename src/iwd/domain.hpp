#ifndef IWD__DOMAIN_HPP
#define IWD__DOMAIN_HPP

#include <iwd/cmake_program.hpp>
#include <iwd/directories.hpp>
#include <iwd/executable.hpp>
#include <memory>
#include <vn/directory.hpp>

namespace iwd {

struct domain
{
  explicit domain(const vn::directory& iwd_directory);

  const directories& dirs() const noexcept;
  const cmake_program& cmake() const noexcept;
  const executable& git() const noexcept;

private:
  directories _directories;
  cmake_program _cmake;
  executable _git;
};

} // namespace iwd

#endif
