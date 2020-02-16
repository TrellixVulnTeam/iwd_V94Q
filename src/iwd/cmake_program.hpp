#ifndef IWD__CMAKE_PROGRAM_HPP
#define IWD__CMAKE_PROGRAM_HPP

#include <filesystem>
#include <iwd/cmake_configuration.hpp>
#include <iwd/executable.hpp>
#include <vn/directory.hpp>
#include <vn/semantic_version.hpp>

namespace iwd {

struct cmake_program
{
  static cmake_program create(executable);

  const vn::semantic_version& version() const noexcept;

  void configure(
    const vn::directory& source_directory,
    const vn::directory& build_directory,
    const cmake_configuration& config) const;

  void install(const vn::directory& build_directory) const;

private:
  cmake_program(executable exec, vn::semantic_version version);

  executable _cmake_executable;
  vn::semantic_version _version;
};

} // namespace iwd

#endif
