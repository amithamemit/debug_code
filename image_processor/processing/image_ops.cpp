#include "image_ops.h"
#include <algorithm>

namespace common {
namespace processing {

void apply_grayscale(std::vector<uint8_t>& image_data) {
    for (size_t i = 0; i < image_data.size(); i += 4) {
        // Standard luminance formula: 0.299R + 0.587G + 0.114B
        uint8_t gray = static_cast<uint8_t>(
            0.299 * image_data[i] + 
            0.587 * image_data[i+1] + 
            0.114 * image_data[i+2]
        );
        image_data[i] = image_data[i+1] = image_data[i+2] = gray;
    }
}

void apply_flip_horizontal(std::vector<uint8_t>& image_data, uint32_t width, uint32_t height) {
    size_t row_size = width * 4;
    for (uint32_t y = 0; y < height; ++y) {
        uint8_t* row = &image_data[y * row_size];
        for (uint32_t x = 0; x < width / 2; ++x) {
            for (int k = 0; k < 4; ++k) {
                std::swap(row[x * 4 + k], row[(width - 1 - x) * 4 + k]);
            }
        }
    }
}

void apply_flip_vertical(std::vector<uint8_t>& image_data, uint32_t width, uint32_t height) {
    size_t row_size = width * 4;
    std::vector<uint8_t> temp_row(row_size);
    for (uint32_t y = 0; y < height / 2; ++y) {
        uint8_t* row_top = &image_data[y * row_size];
        uint8_t* row_bottom = &image_data[(height - 1 - y) * row_size];
        std::copy(row_top, row_top + row_size, temp_row.begin());
        std::copy(row_bottom, row_bottom + row_size, row_top);
        std::copy(temp_row.begin(), temp_row.end(), row_bottom);
    }
}

void apply_invert(std::vector<uint8_t>& image_data) {
    for (size_t i = 0; i < image_data.size(); i += 4) {
        image_data[i]   = 255 - image_data[i];
        image_data[i+1] = 255 - image_data[i+1];
        image_data[i+2] = 255 - image_data[i+2];
    }
}

void apply_brightness(std::vector<uint8_t>& image_data, int16_t factor) {
    for (size_t i = 0; i < image_data.size(); i += 4) {
        for (int k = 0; k < 3; ++k) { // RGB only
            int16_t val = static_cast<int16_t>(image_data[i + k]) + factor;
            image_data[i + k] = static_cast<uint8_t>(std::max<int16_t>(0, std::min<int16_t>(255, val)));
        }
    }
}

} // namespace processing
} // namespace common
