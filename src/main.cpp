#include <filesystem>
#include <nlohmann/json.hpp>

#include "iwd/cmake_configuration.hpp"
#include "iwd/directories.hpp"
#include "iwd/download_file.hpp"
#include "iwd/git_clone.hpp"
#include "iwd/parse_args.hpp"
#include "iwd/requirements.hpp"
#include <vn/string_utils.hpp>

#include <iostream>

namespace nl = nlohmann;

int
main(int argc, const char** argv)
{
  const auto args = iwd::parse_args(argc, argv);
  const auto directories = iwd::directories(args.build_directory);
  const auto requirements = iwd::parse_requirements("iwd.json");
  auto configuration =
    iwd::cmake_configuration::from_arguments(args.cmake_arguments);

  for (const auto& req : requirements) {
    iwd::requirement_handler handler(directories, req);
    handler.download();
  }

  return 0;
}
