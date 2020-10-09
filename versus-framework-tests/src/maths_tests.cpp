#include "vsf.h"
#include "catch2/catch.hpp"

TEST_CASE("math::round_to_int rounds up correctly", "[maths]")
{
    constexpr auto EXPECTED = 1;
    auto result = vsf::maths::round_to_int(0.5f);
    REQUIRE(result == EXPECTED);
}

TEST_CASE("math::round_to_int rounds down correctly", "[maths]")
{
    constexpr auto EXPECTED = 0;
    auto result = vsf::maths::round_to_int(0.499f);
    REQUIRE(result == EXPECTED);
}

TEST_CASE("Rect::width correctly calculates width", "[maths]") 
{
    constexpr auto EXPECTED_WIDTH = 50.23f;
    
    vsf::Rect rect;
    rect.min_x = 100.0f;
    rect.max_x = 150.23f;

    REQUIRE(rect.width() == Approx(EXPECTED_WIDTH));
}

TEST_CASE("Rect::width correctly calculates negative width", "[maths]")
{
    constexpr auto EXPECTED_WIDTH = -50.23f;

    vsf::Rect rect;
    rect.min_x = 150.23f;
    rect.max_x = 100.0f;

    REQUIRE(rect.width() == Approx(EXPECTED_WIDTH));
}

TEST_CASE("Rect::height correctly calculates height", "[maths]")
{
    constexpr auto EXPECTED_HEIGHT = 50.23f;

    vsf::Rect rect;
    rect.min_y = 100.0f;
    rect.max_y = 150.23f;

    REQUIRE(rect.height() == Approx(EXPECTED_HEIGHT));
}

TEST_CASE("Rect::height correctly calculates negative height", "[maths]")
{
    constexpr auto EXPECTED_HEIGHT = -50.23f;

    vsf::Rect rect;
    rect.min_y = 150.23f;
    rect.max_y = 100.0f;

    REQUIRE(rect.height() == Approx(EXPECTED_HEIGHT));
}

TEST_CASE("RectInt::width correctly calculates width", "[maths]")
{
    constexpr auto EXPECTED_WIDTH = 50;

    vsf::RectInt rect;
    rect.min_x = 100;
    rect.max_x = 150;

    REQUIRE(rect.width() == Approx(EXPECTED_WIDTH));
}

TEST_CASE("RectInt::width correctly calculates negative width", "[maths]")
{
    constexpr auto EXPECTED_WIDTH = -50;

    vsf::RectInt rect;
    rect.min_x = 150;
    rect.max_x = 100;

    REQUIRE(rect.width() == Approx(EXPECTED_WIDTH));
}

TEST_CASE("RectInt::height correctly calculates height", "[maths]")
{
    constexpr auto EXPECTED_HEIGHT = 50;

    vsf::RectInt rect;
    rect.min_y = 100;
    rect.max_y = 150;

    REQUIRE(rect.height() == Approx(EXPECTED_HEIGHT));
}



TEST_CASE("RectInt::height correctly calculates negative height", "[maths]")
{
    constexpr auto EXPECTED_HEIGHT = -50;

    vsf::RectInt rect;
    rect.min_y = 150;
    rect.max_y = 100;

    REQUIRE(rect.height() == Approx(EXPECTED_HEIGHT));
}
