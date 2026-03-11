#include "catch.hpp"
#include "../image_message.h"
#include <vector>

using namespace common::icd;

TEST_CASE("ImageMessage Serialization/Deserialization", "[common][icd]") {
    SECTION("Header serialization") {
        ImageMessageHeader header;
        std::memset(&header, 0, sizeof(header));
        std::strcpy(header.filename, "test.png");
        header.width = 100;
        header.height = 200;
        header.operations = OP_GRAYSCALE | OP_INVERT;
        header.output_method = static_cast<uint8_t>(OutputMethod::FILE);

        ImageMessage msg;
        msg.header = header;
        msg.image_data = {1, 2, 3, 4, 5};

        std::vector<uint8_t> buffer = msg.serialize();
        
        REQUIRE(buffer.size() == sizeof(ImageMessageHeader) + 5);
        
        ImageMessage deserialized = ImageMessage::deserialize(buffer);
        
        CHECK(std::string(deserialized.header.filename) == "test.png");
        CHECK(deserialized.header.width == 100);
        CHECK(deserialized.header.height == 200);
        CHECK(deserialized.header.operations == (OP_GRAYSCALE | OP_INVERT));
        CHECK(deserialized.header.output_method == static_cast<uint8_t>(OutputMethod::FILE));
        CHECK(deserialized.image_data == msg.image_data);
    }

    SECTION("Empty data serialization") {
        ImageMessage msg;
        std::memset(&msg.header, 0, sizeof(msg.header));
        msg.image_data.clear();

        std::vector<uint8_t> buffer = msg.serialize();
        REQUIRE(buffer.size() == sizeof(ImageMessageHeader));

        ImageMessage deserialized = ImageMessage::deserialize(buffer);
        CHECK(deserialized.image_data.empty());
    }
}
