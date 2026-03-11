#include "catch.hpp"
#include "../processing/image_ops.h"
#include <vector>

using namespace common::processing;

TEST_CASE("Image Operations", "[image_processor]") {
    SECTION("Apply Invert") {
        std::vector<uint8_t> data = {10, 20, 30, 255, 100, 150, 200, 128};
        apply_invert(data);
        
        CHECK(data[0] == 255 - 10);
        CHECK(data[1] == 255 - 20);
        CHECK(data[2] == 255 - 30);
        CHECK(data[3] == 255); // Alpha stays same
        
        CHECK(data[4] == 255 - 100);
        CHECK(data[5] == 255 - 150);
        CHECK(data[6] == 255 - 200);
        CHECK(data[7] == 128); // Alpha stays same
    }

    SECTION("Apply Grayscale") {
        std::vector<uint8_t> data = {100, 100, 100, 255, 0, 255, 0, 255};
        apply_grayscale(data);
        
        // Grayscale for 100, 100, 100 should be 100
        // Formula: 0.299R + 0.587G + 0.114B
        uint8_t g1 = static_cast<uint8_t>(0.299 * 100 + 0.587 * 100 + 0.114 * 100);
        CHECK((int)data[0] >= (int)g1 - 1);
        CHECK((int)data[0] <= (int)g1 + 1);
        CHECK((int)data[1] >= (int)g1 - 1);
        CHECK((int)data[1] <= (int)g1 + 1);
        CHECK((int)data[2] >= (int)g1 - 1);
        CHECK((int)data[2] <= (int)g1 + 1);
        
        // Grayscale for 0, 255, 0
        uint8_t g2 = static_cast<uint8_t>(0.299 * 0 + 0.587 * 255 + 0.114 * 0);
        CHECK((int)data[4] >= (int)g2 - 1);
        CHECK((int)data[4] <= (int)g2 + 1);
    }

    SECTION("Apply Brightness") {
        std::vector<uint8_t> data = {100, 100, 100, 255};
        apply_brightness(data, 50);
        CHECK(data[0] == 150);
        CHECK(data[1] == 150);
        CHECK(data[2] == 150);
        
        apply_brightness(data, 200); // Should cap at 255
        CHECK(data[0] == 255);
    }
}
