#include <nlohmann/json.hpp>
#include <filesystem>

#include "iwd/directories.hpp"
#include "iwd/parse_args.hpp"
#include "iwd/requirements.hpp"

namespace nl = nlohmann;

int
main(int argc, const char** argv)
{
  const auto args = iwd::parse_args(argc, argv);
  const auto directories = iwd::directories(args.build_directory);
  const auto requirements = iwd::parse_requirements("iwd.json");

  nl::json json;
  json["hello"] = "world";

  return 0;
}
