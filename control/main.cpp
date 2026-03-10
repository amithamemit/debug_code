#include "common/ipc/mq_wrapper.h"
#include "common/icd/image_message.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

/**
 * @brief Parses the comma-separated filters string into a bitmask.
 */
uint32_t parse_filters(const std::string& filters_str) {
    using namespace common::icd;
    uint32_t ops = OP_NONE;
    std::stringstream ss(filters_str);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // Trim potential whitespace
        item.erase(0, item.find_first_not_of(" "));
        item.erase(item.find_last_not_of(" ") + 1);

        if (item == "grayscale") ops |= OP_GRAYSCALE;
        else if (item == "flip_h") ops |= OP_FLIP_HORIZONTAL;
        else if (item == "flip_v") ops |= OP_FLIP_VERTICAL;
        else if (item == "invert") ops |= OP_INVERT;
        else if (item == "brightness") ops |= OP_BRIGHTNESS;
        else if (item == "blur") ops |= OP_BLUR;
        else if (item == "sharpen") ops |= OP_SHARPEN;
        else if (item != "none" && !item.empty()) {
            std::cerr << "[Control] Warning: Unknown filter '" << item << "'" << std::endl;
        }
    }
    return ops;
}

int main() {
    using namespace common;

    std::cout << "[Control] Persistent Interactive Session started." << std::endl;
    std::cout << "[Control] Connecting to 'format_in'..." << std::endl;

    // 1. Maintain the connection throughout the session
    ipc::MessageQueue output_q("format_in", ipc::MessageQueue::Mode::SENDER);

    while (true) {
        std::cout << "\n--------------------------------------------------" << std::endl;
        std::cout << "Enter command (filename filters output_method) or 'help' or 'exit':" << std::endl;
        std::cout << "> ";

        std::string input_line;
        if (!std::getline(std::cin, input_line) || input_line == "exit") {
            break;
        }

        if (input_line == "help") {
            std::cout << "\n[Help] Image Processing System Control" << std::endl;
            std::cout << "Usage: <filename> <filters> <output_method>" << std::endl;
            std::cout << "Example: input.png grayscale,invert file" << std::endl;
            std::cout << "\nAvailable Filters (comma-separated):" << std::endl;
            std::cout << "  - grayscale : Convert to black and white" << std::endl;
            std::cout << "  - flip_h    : Flip horizontally" << std::endl;
            std::cout << "  - flip_v    : Flip vertically" << std::endl;
            std::cout << "  - invert    : Invert colors" << std::endl;
            std::cout << "  - brightness: Increase brightness (+20)" << std::endl;
            std::cout << "  - blur      : Apply blur effect (Not implemented)" << std::endl;
            std::cout << "  - sharpen   : Apply sharpen effect (Not implemented)" << std::endl;
            std::cout << "  - none      : No filters" << std::endl;
            std::cout << "\nAvailable Output Methods:" << std::endl;
            std::cout << "  - screen    : Print metadata to console" << std::endl;
            std::cout << "  - file      : Save as [filename]_modified.png" << std::endl;
            continue;
        }

        std::stringstream ss(input_line);
        std::string filename, filters_str, output_str;

        if (!(ss >> filename >> filters_str >> output_str)) {
            if (!filename.empty()) {
                std::cerr << "[Control] Invalid input format. Expected: <filename> <filters> <output_method>" << std::endl;
            }
            continue;
        }

        // 2. Prepare the message header
        icd::ImageMessageHeader header;
        std::memset(&header, 0, sizeof(header));
        
        std::strncpy(header.filename, filename.c_str(), sizeof(header.filename) - 1);
        header.operations = parse_filters(filters_str);
        
        if (output_str == "file") {
            header.output_method = static_cast<uint8_t>(icd::OutputMethod::FILE);
        } else {
            header.output_method = static_cast<uint8_t>(icd::OutputMethod::SCREEN);
        }

        icd::ImageMessage msg;
        msg.header = header;

        // 3. Send to format_in
        std::cout << "[Control] Requesting: " << filename << " [" << filters_str << "] -> " << output_str << std::endl;

        std::vector<uint8_t> buffer = msg.serialize();
        if (output_q.send(buffer)) {
            std::cout << "[Control] Message sent to pipeline." << std::endl;
        } else {
            std::cerr << "[Control] Error: Failed to send message. Pipeline might be down." << std::endl;
            break;
        }
    }

    std::cout << "[Control] Session ended. Exiting." << std::endl;
    return 0;
}
