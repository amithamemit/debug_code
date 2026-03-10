#ifndef COMMON_PROCESSING_IMAGE_OPS_H
#define COMMON_PROCESSING_IMAGE_OPS_H

#include <vector>
#include <cstdint>

namespace common {
namespace processing {

/**
 * @brief Converts an RGBA image to grayscale (in-place).
 */
void apply_grayscale(std::vector<uint8_t>& image_data);

/**
 * @brief Flips an RGBA image horizontally.
 */
void apply_flip_horizontal(std::vector<uint8_t>& image_data, uint32_t width, uint32_t height);

/**
 * @brief Flips an RGBA image vertically.
 */
void apply_flip_vertical(std::vector<uint8_t>& image_data, uint32_t width, uint32_t height);

/**
 * @brief Inverts the colors of an RGBA image (RGBA -> (255-R, 255-G, 255-B, A)).
 */
void apply_invert(std::vector<uint8_t>& image_data);

/**
 * @brief Adjusts the brightness of an RGBA image.
 * @param factor Positive to brighten, negative to darken.
 */
void apply_brightness(std::vector<uint8_t>& image_data, int16_t factor);

} // namespace processing
} // namespace common

#endif // COMMON_PROCESSING_IMAGE_OPS_H
