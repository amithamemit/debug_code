#include "png_parser.h"
#include "lodepng/lodepng.h"
#include "common/icd/log_message.h"

namespace common {
namespace parsing {

std::vector<uint8_t> decode_png(const std::string& filename, unsigned int& width, unsigned int& height) {
    std::vector<uint8_t> image;
    unsigned int error = lodepng::decode(image, width, height, filename);

    if (error) {
        std::string err_msg = "LodePNG decode error " + std::to_string(error) + ": " + lodepng_error_text(error);
        icd::send_log(icd::LogOpcode::ERROR_DECODE, err_msg);
        return {};
    }

    return image;
}

bool encode_png(const std::string& filename, const std::vector<uint8_t>& image, unsigned int width, unsigned int height) {
    unsigned int error = lodepng::encode(filename, image, width, height);

    if (error) {
        std::string err_msg = "LodePNG encode error " + std::to_string(error) + ": " + lodepng_error_text(error);
        icd::send_log(icd::LogOpcode::ERROR_ENCODE, err_msg);
        return false;
    }

    return true;
}

} // namespace parsing
} // namespace common
