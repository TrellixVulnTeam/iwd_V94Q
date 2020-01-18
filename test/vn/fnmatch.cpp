#include <catch2/catch.hpp>

#include <utility>
#include <vn/fnmatch.hpp>

TEST_CASE("fnmatch", "[fnmatch]")
{
  auto [fname, pattern, should_match] = GENERATE(
    // Test cases taken from python
    std::make_tuple("abc", "abc", true),
    std::make_tuple("abc", "?*?", true),
    std::make_tuple("abc", "???*", true),
    std::make_tuple("abc", "*???", true),
    std::make_tuple("abc", "???", true),
    std::make_tuple("abc", "*", true),
    std::make_tuple("abc", "ab[cd]", true),
    std::make_tuple("abc", "ab[!de]", true),
    std::make_tuple("abc", "ab[de]", false),
    std::make_tuple("a", "??", false),
    std::make_tuple("a", "b", false),
    // Custom ones
    std::make_tuple("a/b.cpp", "a/*.cpp", true),
    std::make_tuple("a/b.cpp", "a/*.hpp", false),
    std::make_tuple("a/b/c.cpp", "a/*/c.cpp", true),
    std::make_tuple("a/b/c/d.cpp", "a/*/d.cpp", false));

  vn::fnmatch match(pattern);
  INFO(
    "Pattern: " << std::quoted(pattern) << ", fname:" << std::quoted(fname)
                << ", should match:" << should_match);
  REQUIRE(match(fname) == should_match);
}
