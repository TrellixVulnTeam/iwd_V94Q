#include <catch2/catch.hpp>

#include <vn/string_template.hpp>

using namespace Catch::Matchers;

TEST_CASE(
  "String templating throws on empty string in ctor",
  "[string_template]")
{
  REQUIRE_THROWS_AS(
    vn::string_template(std::string_view()), std::invalid_argument);
}

TEST_CASE("Throws on empty key", "[string_template]")
{
  vn::string_template str_template("My name is $(NAME)");
  REQUIRE_THROWS_WITH(
    str_template.render(std::map<std::string, std::string>{}),
    Contains("NAME"));
}

TEST_CASE("Formats properly", "[string_template]")
{
  vn::string_template t1("My name is $(NAME)");
  REQUIRE(t1.render({ { "NAME", "John" } }) == "My name is John");

  vn::string_template t2(
    "My name is $(NAME) and i like $(LIKE) amongst many things");
  REQUIRE(
    t2.render({ { "NAME", "John" }, { "LIKE", "fun" } }) ==
    "My name is John and i like fun amongst many things");
}
