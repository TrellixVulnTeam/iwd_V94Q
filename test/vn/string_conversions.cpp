#include <catch2/catch.hpp>

#include <vn/string_conversions.hpp>

TEST_CASE("try convert", "[string_conversions]")
{
  REQUIRE(vn::try_convert<int>("0").has_value());
  REQUIRE(vn::try_convert<int>("0").value() == 0);
  REQUIRE(vn::try_convert<int>("zxczxc").has_value() == false);
  REQUIRE(vn::try_convert<int>("123123").has_value());
  REQUIRE(vn::try_convert<int>("123123").value() == 123123);
}
