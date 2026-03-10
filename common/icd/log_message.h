#ifndef COMMON_ICD_LOG_MESSAGE_H
#define COMMON_ICD_LOG_MESSAGE_H

#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>

namespace common {
namespace icd {

/**
 * @brief Enum representing the opcode (severity/type) of a log or error.
 */
enum class LogOpcode : uint32_t {
    INFO           = 0,
    WARNING        = 1,
    ERROR_DECODE   = 2,
    ERROR_ENCODE   = 3,
    ERROR_PROCESS  = 4,
    ERROR_IPC      = 5,
    DEBUG          = 6
};

#pragma pack(push, 1)
/**
 * @brief Fixed-size header for log messages.
 */
struct LogMessageHeader {
    uint64_t timestamp;      /**< Unix timestamp of the log entry */
    uint32_t opcode;         /**< LogOpcode representing the type of log */
    uint32_t message_length; /**< Length of the following message string */
};
#pragma pack(pop)

/**
 * @brief Structure representing a log message with header and variable-length text.
 */
struct LogMessage {
    LogMessageHeader header;
    std::string message;

    /**
     * @brief Builds a LogMessage with the current timestamp and provided data.
     */
    static LogMessage build(LogOpcode opcode, const std::string& msg);

    /**
     * @brief Serializes the LogMessage into a single byte buffer for transmission.
     */
    std::vector<uint8_t> serialize() const;

    /**
     * @brief Deserializes a byte buffer back into a LogMessage object.
     */
    static LogMessage deserialize(const std::vector<uint8_t>& buffer);
};

/**
 * @brief Sends a log message to the log process.
 * @param log The pre-built log message.
 */
void send_log(const LogMessage& log);

/**
 * @brief Convenience function to build and send a log message in one step.
 * @param opcode The log opcode.
 * @param msg The message text (optional).
 */
void send_log(LogOpcode opcode, const std::string& msg = "");

} // namespace icd
} // namespace common

#endif // COMMON_ICD_LOG_MESSAGE_H
