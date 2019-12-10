#include <filesystem>
#include "argparse.hpp"
#include <nlohmann/json.hpp>

namespace nl = nlohmann;

struct directories
{
    std::filesystem::path cache_directory;
    std::filesystem::path output_directory;
};



int main(int argc, const char ** argv)
{
    auto parser = argparse::ArgumentParser();
    parser.add_argument("hello");

    nl::json json;
    json["hello"] =  "world";

    try {
      parser.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
      std::cout << err.what() << std::endl;
      std::cout << parser;
      exit(0);
    }

    directories dirs;
    dirs.cache_directory = std::filesystem::current_path();
    return 0;
}
