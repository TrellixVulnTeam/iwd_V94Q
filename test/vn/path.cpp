#include <catch2/catch.hpp>

#include <tuple>
#include <vn/path.hpp>

std::tuple<
  std::filesystem::path,
  std::optional<std::filesystem::path>,
  std::optional<std::filesystem::path>>
make_split_path_test_entry(
  const std::filesystem::path& fs,
  const std::optional<std::filesystem::path>& directory,
  const std::optional<std::filesystem::path>& fname)
{
  return std::make_tuple(fs, directory, fname);
}

TEST_CASE("split path", "[path]")
{
  auto [target, should_be_directory, should_be_fname] = GENERATE(
    make_split_path_test_entry(
      std::filesystem::path("/foo/bar"),
      std::filesystem::path("/foo"),
      std::filesystem::path("bar")),
    make_split_path_test_entry(
      std::filesystem::path("/"), std::filesystem::path("/"), std::nullopt),
    make_split_path_test_entry(
      std::filesystem::path("foo"), std::nullopt, std::filesystem::path("foo")),
    make_split_path_test_entry(
      std::filesystem::path("////foo"),
      std::filesystem::path("////"),
      std::filesystem::path("foo")),
    make_split_path_test_entry(
      std::filesystem::path("//foo//bar"),
      std::filesystem::path("//foo"),
      std::filesystem::path("bar")));

  const auto [directory, fname] = vn::split_path(target);
  REQUIRE(should_be_directory == directory);
  REQUIRE(should_be_fname == fname);
}
