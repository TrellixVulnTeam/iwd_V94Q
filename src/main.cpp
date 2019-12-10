#include <nlohmann/json.hpp>
#include <filesystem>

#include "iwd/parse_args.hpp"

namespace nl = nlohmann;

struct directories
{
  std::filesystem::path cache_directory;
  std::filesystem::path output_directory;
};

int
main(int argc, const char** argv)
{
  const auto args = iwd::parse_args(argc, argv);

  nl::json json;
  json["hello"] = "world";

  directories dirs;
  dirs.cache_directory = std::filesystem::current_path();
  return 0;
}
