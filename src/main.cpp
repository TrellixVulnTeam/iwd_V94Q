#include <filesystem>

struct directories
{
    std::filesystem::path cache_directory;
    std::filesystem::path output_directory;
};



int main(int argc, const char ** argv)
{
    directories dirs;
    dirs.cache_directory = std::filesystem::current_path();
    return 0;
}
