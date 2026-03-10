#include "log_message.h"
#include <iostream>

namespace common {
namespace icd {

LogMessage LogMessage::build(LogOpcode opcode, const std::string& msg) {
    LogMessage log;
    log.header.timestamp = static_cast<uint64_t>(std::time(nullptr));
    log.header.opcode = static_cast<uint32_t>(opcode);
    log.header.message_length = static_cast<uint32_t>(msg.length());
    log.message = msg;
    return log;
}

std::vector<uint8_t> LogMessage::serialize() const {
    std::vector<uint8_t> buffer(sizeof(LogMessageHeader) + message.length());
    
    std::memcpy(buffer.data(), &header, sizeof(LogMessageHeader));
    if (!message.empty()) {
        std::memcpy(buffer.data() + sizeof(LogMessageHeader), message.data(), message.length());
    }
    
    return buffer;
}

LogMessage LogMessage::deserialize(const std::vector<uint8_t>& buffer) {
    LogMessage log;
    if (buffer.size() < sizeof(LogMessageHeader)) return log;

    std::memcpy(&log.header, buffer.data(), sizeof(LogMessageHeader));
    
    if (buffer.size() > sizeof(LogMessageHeader)) {
        uint32_t actual_data_len = static_cast<uint32_t>(buffer.size() - sizeof(LogMessageHeader));
        uint32_t read_len = (log.header.message_length < actual_data_len) ? log.header.message_length : actual_data_len;
        
        log.message.assign(reinterpret_cast<const char*>(buffer.data() + sizeof(LogMessageHeader)), read_len);
    }

    return log;
}

void send_log(const LogMessage& log) {
    // Serialize and "Send" to the Rust process (simulated)
    std::vector<uint8_t> serialized_data = log.serialize();
    
    std::cout << "[IPC] Sent log message to Rust process: "
              << "Time: " << log.header.timestamp 
              << ", Opcode: " << log.header.opcode 
              << ", Msg: " << log.message << std::endl;
}

void send_log(LogOpcode opcode, const std::string& msg) {
    // Build and send
    LogMessage log = LogMessage::build(opcode, msg);
    send_log(log);
}

} // namespace icd
} // namespace common
