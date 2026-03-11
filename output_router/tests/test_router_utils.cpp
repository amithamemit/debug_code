#include "catch.hpp"
#include <string>

#define main router_main
#include "../main.cpp"
#undef main

TEST_CASE("Router Utils: get_modified_filename", "[output_router]") {
    SECTION("Normal filename") {
        CHECK(get_modified_filename("test.png") == "test_modified.png");
        CHECK(get_modified_filename("image.jpg") == "image_modified.jpg");
    }

    SECTION("No extension") {
        CHECK(get_modified_filename("test") == "test_modified");
    }

    SECTION("Multiple dots") {
        CHECK(get_modified_filename("archive.tar.gz") == "archive.tar_modified.gz");
    }
}
