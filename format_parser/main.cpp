#include "common/ipc/mq_wrapper.h"
#include "common/icd/image_message.h"
#include "common/parsing/png_parser.h"
#include "common/icd/log_message.h"
#include <iostream>
#include <vector>

/**
 * @brief The format_parser process.
 * 
 * Responsibilities:
 * 1. Listen for ImageMessage requests on the "format_in" MQ.
 * 2. Read the image file specified in the message.
 * 3. Decode the PNG using the parsing utility.
 * 4. Forward the complete message (header + pixels) to the "image_proc_in" MQ.
 */
int main() {
    using namespace common;

    // Initialize IPC Queues
    // "format_in" receives initial requests (containing filename and operations)
    ipc::MessageQueue input_q("format_in", ipc::MessageQueue::Mode::RECEIVER);
    
    // "image_proc_in" sends enriched messages to the image_processor
    ipc::MessageQueue output_q("image_proc_in", ipc::MessageQueue::Mode::SENDER);

    std::cout << "[FormatParser] Process started." << std::endl;
    std::cout << "[FormatParser] Listening on 'format_in' for image requests..." << std::endl;

    while (true) {
        std::vector<uint8_t> buffer;
        if (input_q.receive(buffer)) {
            // 1. Deserialize the incoming message (expected to contain header)
            icd::ImageMessage msg = icd::ImageMessage::deserialize(buffer);
            
            std::cout << "[FormatParser] Received request for file: " << msg.header.filename << std::endl;

            // 2. Decode the PNG file using the parsing utility
            unsigned int width = 0, height = 0;
            std::vector<uint8_t> pixel_data = parsing::decode_png(msg.header.filename, width, height);

            if (pixel_data.empty()) {
                // Note: parsing::decode_png already sends a log message on failure
                std::cerr << "[FormatParser] Failed to decode: " << msg.header.filename << std::endl;
                continue;
            }

            // 3. Update the message with image details and raw data
            msg.header.width = width;
            msg.header.height = height;
            msg.image_data = pixel_data;

            // 4. Forward the enriched message to the image_processor
            std::vector<uint8_t> output_buffer = msg.serialize();
            if (output_q.send(output_buffer)) {
                std::cout << "[FormatParser] Successfully forwarded " << output_buffer.size() 
                          << " bytes of image data to 'image_proc_in'." << std::endl;
            } else {
                icd::send_log(icd::LogOpcode::ERROR_IPC, "FormatParser failed to forward message to image_processor");
                std::cerr << "[FormatParser] IPC Error: Failed to send to image_processor" << std::endl;
            }
        } else {
            // Check for IPC errors or termination
            break;
        }
    }

    std::cout << "[FormatParser] Process exiting." << std::endl;
    return 0;
}
