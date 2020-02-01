#ifndef IWD__DOMAIN_HPP
#define IWD__DOMAIN_HPP

#include <iwd/directories.hpp>
#include <iwd/executable.hpp>
#include <memory>
#include <vn/directory.hpp>

namespace iwd {

struct domain
{
  explicit domain(const vn::directory& iwd_directory);

  const directories& dirs() const noexcept;
  const executable& cmake() const noexcept;
  const executable& git() const noexcept;

private:
  directories _directories;
  executable _cmake;
  executable _git;
};

} // namespace iwd

#endif
