#include "common/ipc/mq_wrapper.h"
#include "common/icd/image_message.h"
#include "processing/image_ops.h"
#include "common/icd/log_message.h"
#include <iostream>
#include <vector>

/**
 * @brief The image_processor process.
 * 
 * Responsibilities:
 * 1. Listen for ImageMessage on the "image_proc_in" MQ.
 * 2. Apply requested operations (Grayscale, Flip, etc.) from message header.
 * 3. Forward the updated ImageMessage to the "output_router_in" MQ.
 */
int main() {
    using namespace common;

    // Initialize IPC Queues
    ipc::MessageQueue input_q("image_proc_in", ipc::MessageQueue::Mode::RECEIVER);
    ipc::MessageQueue output_q("output_router_in", ipc::MessageQueue::Mode::SENDER);

    std::cout << "[ImageProcessor] Process started." << std::endl;
    std::cout << "[ImageProcessor] Listening on 'image_proc_in'..." << std::endl;

    while (true) {
        std::vector<uint8_t> buffer;
        if (input_q.receive(buffer)) {
            // 1. Deserialize the message from format_parser
            icd::ImageMessage msg = icd::ImageMessage::deserialize(buffer);
            
            std::cout << "[ImageProcessor] Processing file: " << msg.header.filename 
                      << " (Operations: " << std::hex << msg.header.operations << std::dec << ")" << std::endl;

            // 2. Apply operations based on bitmask
            if (msg.header.operations == icd::OP_GRAYSCALE) {
                processing::apply_grayscale(msg.image_data);
            }
            else if (msg.header.operations == icd::OP_FLIP_HORIZONTAL) {
                processing::apply_flip_horizontal(msg.image_data, msg.header.width, msg.header.height);
            }
            else if (msg.header.operations == icd::OP_FLIP_VERTICAL) {
                processing::apply_flip_vertical(msg.image_data, msg.header.width, msg.header.height);
            }
            else if (msg.header.operations == icd::OP_INVERT) {
                processing::apply_invert(msg.image_data);
            }
            else if (msg.header.operations == icd::OP_BRIGHTNESS) {
                // Defaulting to +20 brightness for this demonstration
                processing::apply_brightness(msg.image_data, 20);
            }
            else{
                // no filter to be made, continuing to next file without appllying filters 
                continue;
            }

            // 3. Forward to the output_router
            std::vector<uint8_t> output_buffer = msg.serialize();
            if (output_q.send(output_buffer)) {
                std::cout << "[ImageProcessor] Successfully forwarded result to 'output_router_in'." << std::endl;
            } else {
                icd::send_log(icd::LogOpcode::ERROR_IPC, "ImageProcessor failed to forward message to output_router");
                std::cerr << "[ImageProcessor] IPC Error: Failed to send to output_router" << std::endl;
            }
        } else {
            break;
        }
    }

    std::cout << "[ImageProcessor] Process exiting." << std::endl;
    return 0;
}
