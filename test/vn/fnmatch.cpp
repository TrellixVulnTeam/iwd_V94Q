#include <catch2/catch.hpp>

#include <regex>
#include <tuple>
#include <utility>
#include <vn/fnmatch.hpp>

TEST_CASE("fnmatch-translate", "[fnmatch]")
{
  REQUIRE(vn::detail::translate_fnmatch_expr("*") == R"re(.*)re");
  REQUIRE(vn::detail::translate_fnmatch_expr("?") == R"re(.)re");
  REQUIRE(vn::detail::translate_fnmatch_expr("a?b*") == R"re(a.b.*)re");
  REQUIRE(vn::detail::translate_fnmatch_expr("[abc]") == R"re([abc])re");
  REQUIRE(vn::detail::translate_fnmatch_expr("[]]") == R"re([]])re");
  REQUIRE(vn::detail::translate_fnmatch_expr("[!x]") == R"re([^x])re");
  REQUIRE(vn::detail::translate_fnmatch_expr("[^x]") == R"re([\^x])re");
  REQUIRE(vn::detail::translate_fnmatch_expr("[x") == R"re(\[x)re");
  REQUIRE(vn::detail::translate_fnmatch_expr("ab[cd]") == R"re(ab[cd])re");
}

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
    std::make_tuple("a/b/c/d.cpp", "a/*/d.cpp", true));

  vn::fnmatch match(pattern);
  const auto translatedPattern = vn::detail::translate_fnmatch_expr(pattern);

  INFO(
    "Pattern: " << std::quoted(pattern)
                << ",translation:" << std::quoted(translatedPattern)
                << ", fname:" << std::quoted(fname)
                << ", should match:" << should_match);
  REQUIRE(match(fname) == should_match);
}
