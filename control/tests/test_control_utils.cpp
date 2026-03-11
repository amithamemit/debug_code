#include "catch.hpp"
#include <string>
#include <cstdint>

// Forward declaration of the function to test
// We'll link against the object file later or include the code if needed
// For now, let's assume we can test it by including the definition or a mock
#define main control_main
#include "../main.cpp"
#undef main

TEST_CASE("Control Utils: parse_filters", "[control]") {
    using namespace common::icd;

    SECTION("Single filter") {
        CHECK(parse_filters("grayscale") == OP_GRAYSCALE);
        CHECK(parse_filters("invert") == OP_INVERT);
    }

    SECTION("Multiple filters") {
        CHECK(parse_filters("grayscale,invert") == (OP_GRAYSCALE | OP_INVERT));
        CHECK(parse_filters(" flip_h , flip_v ") == (OP_FLIP_HORIZONTAL | OP_FLIP_VERTICAL));
    }

    SECTION("Unknown or empty filters") {
        CHECK(parse_filters("") == OP_NONE);
        CHECK(parse_filters("unknown,none") == OP_NONE);
    }
}
