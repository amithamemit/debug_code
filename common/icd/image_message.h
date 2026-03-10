#ifndef COMMON_ICD_IMAGE_MESSAGE_H
#define COMMON_ICD_IMAGE_MESSAGE_H

#include <cstdint>
#include <vector>
#include <cstring>

namespace common {
namespace icd {

/**
 * @brief Enum for image processing operations.
 * Use as a bitmask to combine multiple operations.
 */
enum ImageOperation : uint32_t {
    OP_NONE            = 0,
    OP_GRAYSCALE       = 1 << 0,
    OP_FLIP_HORIZONTAL = 1 << 1,
    OP_FLIP_VERTICAL   = 1 << 2,
    OP_BLUR            = 1 << 3,
    OP_SHARPEN         = 1 << 4,
    OP_INVERT          = 1 << 5,
    OP_BRIGHTNESS      = 1 << 6
};

/**
 * @brief Enum for the desired output method of the processed image.
 */
enum class OutputMethod : uint8_t {
    SCREEN = 0,
    FILE   = 1
};

#pragma pack(push, 1)
/**
 * @brief Fixed-size header for the image message passing through the system.
 */
struct ImageMessageHeader {
    uint32_t total_length;   /**< Total length of the message (header + image data) */
    char filename[256];      /**< Filename associated with the image */
    uint32_t width;          /**< Image width in pixels */
    uint32_t height;         /**< Image height in pixels */
    uint32_t operations;     /**< Bitmask of ImageOperation to perform */
    uint8_t output_method;   /**< Desired output method (OutputMethod) */
};
#pragma pack(pop)

/**
 * @brief Helper structure to represent a complete message in memory.
 * Note: When sending over IPC, the header and data should be serialized into a single buffer.
 */
struct ImageMessage {
    ImageMessageHeader header;
    std::vector<uint8_t> image_data;

    /**
     * @brief Serializes the ImageMessage into a single byte buffer.
     * @return std::vector<uint8_t> The serialized buffer.
     */
    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer(sizeof(ImageMessageHeader) + image_data.size());
        
        ImageMessageHeader h = header;
        h.total_length = static_cast<uint32_t>(buffer.size());
        
        std::memcpy(buffer.data(), &h, sizeof(ImageMessageHeader));
        std::memcpy(buffer.data() + sizeof(ImageMessageHeader), image_data.data(), image_data.size());
        
        return buffer;
    }

    /**
     * @brief Deserializes a buffer into an ImageMessage.
     * @param buffer The buffer to deserialize.
     * @return ImageMessage The deserialized message.
     */
    static ImageMessage deserialize(const std::vector<uint8_t>& buffer) {
        ImageMessage msg;
        if (buffer.size() < sizeof(ImageMessageHeader)) return msg;

        std::memcpy(&msg.header, buffer.data(), sizeof(ImageMessageHeader));
        
        size_t data_size = buffer.size() - sizeof(ImageMessageHeader);
        if (data_size > 0) {
            msg.image_data.assign(buffer.begin() + sizeof(ImageMessageHeader), buffer.end());
        }

        return msg;
    }
};

} // namespace icd
} // namespace common

#endif // COMMON_ICD_IMAGE_MESSAGE_H
