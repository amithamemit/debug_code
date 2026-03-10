#ifndef COMMON_PARSING_PNG_PARSER_H
#define COMMON_PARSING_PNG_PARSER_H

#include <vector>
#include <string>
#include <cstdint>

namespace common {
namespace parsing {

/**
 * @brief Decodes a PNG image file into raw RGBA data.
 * 
 * @param filename The path to the PNG file.
 * @param width Reference to store the image width.
 * @param height Reference to store the image height.
 * @return std::vector<uint8_t> A buffer containing the raw RGBA pixel data (row data).
 *         Returns an empty vector on failure.
 */
std::vector<uint8_t> decode_png(const std::string& filename, unsigned int& width, unsigned int& height);

/**
 * @brief Serializes raw RGBA data into a PNG file.
 * 
 * @param filename The name of the file to save (including .png extension).
 * @param image The buffer containing raw RGBA pixel data.
 * @param width The width of the image.
 * @param height The height of the image.
 * @return bool True if serialization and saving were successful, false otherwise.
 */
bool encode_png(const std::string& filename, const std::vector<uint8_t>& image, unsigned int width, unsigned int height);

} // namespace parsing
} // namespace common

#endif // COMMON_PARSING_PNG_PARSER_H
