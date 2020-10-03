#include "vsf.h"
#include "catch2/catch.hpp"

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
