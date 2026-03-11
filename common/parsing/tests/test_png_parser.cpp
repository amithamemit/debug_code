#include "catch.hpp"
#include "../png_parser.h"
#include <vector>
#include <fstream>
#include <cstdio>

using namespace common::parsing;

bool file_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

TEST_CASE("PNG Parser", "[common][parsing]") {
    SECTION("Encode and Decode") {
        std::string test_file = "test_temp.png";
        unsigned int width = 2, height = 2;
        std::vector<uint8_t> original_data = {
            255, 0, 0, 255,  // Red
            0, 255, 0, 255,  // Green
            0, 0, 255, 255,  // Blue
            255, 255, 255, 255 // White
        };

        // Test Encode
        bool encode_ok = encode_png(test_file, original_data, width, height);
        REQUIRE(encode_ok);
        REQUIRE(file_exists(test_file));

        // Test Decode
        unsigned int dec_w, dec_h;
        std::vector<uint8_t> decoded_data = decode_png(test_file, dec_w, dec_h);
        
        CHECK(dec_w == width);
        CHECK(dec_h == height);
        CHECK(decoded_data == original_data);

        // Cleanup
        std::remove(test_file.c_str());
    }

    SECTION("Decode non-existent file") {
        unsigned int w, h;
        auto data = decode_png("non_existent.png", w, h);
        CHECK(data.empty());
    }
}
