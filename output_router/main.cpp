#include "common/ipc/mq_wrapper.h"
#include "common/icd/image_message.h"
#include "common/parsing/png_parser.h"
#include "common/icd/log_message.h"
#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Modifies the filename by adding "_modified" before the extension.
 * e.g., "test.png" -> "test_modified.png"
 */
std::string get_modified_filename(const std::string& original) {
    size_t last_dot = original.find_last_of(".");
    if (last_dot == std::string::npos) {
        return original + "_modified";
    }
    return original.substr(0, last_dot) + "_modified" + original.substr(last_dot);
}

/**
 * @brief "Outputs" the image to the screen. 
 * In this console implementation, it prints metadata and a success message.
 */
void output_to_screen(const common::icd::ImageMessage& msg) {
    std::cout << "[OutputRouter] --- Screen Output Simulation ---" << std::endl;
    std::cout << "[OutputRouter] Filename: " << msg.header.filename << std::endl;
    std::cout << "[OutputRouter] Resolution: " << msg.header.width << "x" << msg.header.height << std::endl;
    std::cout << "[OutputRouter] Data Size: " << msg.image_data.size() << " bytes" << std::endl;
    std::cout << "[OutputRouter] Image processing complete. Displaying on screen (simulated)." << std::endl;
    std::cout << "[OutputRouter] --------------------------------" << std::endl;
}

/**
 * @brief Saves the image to a modified filename.
 */
void save_to_file(const common::icd::ImageMessage& msg) {
    std::string new_filename = get_modified_filename(msg.header.filename);
    std::cout << "[OutputRouter] Saving to file: " << new_filename << std::endl;
    
    if (common::parsing::encode_png(new_filename, msg.image_data, msg.header.width, msg.header.height)) {
        std::cout << "[OutputRouter] Successfully saved " << new_filename << std::endl;
    } else {
        common::icd::send_log(common::icd::LogOpcode::ERROR_ENCODE, "Failed to encode/save modified PNG: " + new_filename);
        std::cerr << "[OutputRouter] Error: Failed to save file " << new_filename << std::endl;
    }
}

int main() {
    using namespace common;

    // Initialize IPC Queue
    ipc::MessageQueue input_q("output_router_in", ipc::MessageQueue::Mode::RECEIVER);

    std::cout << "[OutputRouter] Process started." << std::endl;
    std::cout << "[OutputRouter] Listening on 'output_router_in'..." << std::endl;

    while (true) {
        std::vector<uint8_t> buffer;
        if (input_q.receive(buffer)) {
            // 1. Deserialize the message from image_processor
            icd::ImageMessage msg = icd::ImageMessage::deserialize(buffer);
            
            std::cout << "[OutputRouter] Received message for: " << msg.header.filename << std::endl;

            // 2. Select output method
            if (msg.header.output_method == static_cast<uint8_t>(icd::OutputMethod::SCREEN)) {
                output_to_screen(msg);
            } else if (msg.header.output_method == static_cast<uint8_t>(icd::OutputMethod::FILE)) {
                save_to_file(msg);
            } else {
                std::cerr << "[OutputRouter] Unknown output method: " << (int)msg.header.output_method << std::endl;
                icd::send_log(icd::LogOpcode::ERROR_PROCESS, "Unknown output method received in OutputRouter");
            }
        } else {
            break;
        }
    }

    std::cout << "[OutputRouter] Process exiting." << std::endl;
    return 0;
}
