#ifndef COMMON_IPC_MQ_WRAPPER_H
#define COMMON_IPC_MQ_WRAPPER_H

#include <string>
#include <vector>
#include <cstdint>

namespace common {
namespace ipc {

/**
 * @brief A simple cross-platform Message Queue abstraction.
 * For true cross-platform behavior without heavy dependencies, this implementation
 * uses TCP sockets to simulate a message queue.
 */
class MessageQueue {
public:
    enum class Mode {
        SENDER,
        RECEIVER
    };

    /**
     * @brief Construct a Message Queue.
     * @param name A unique identifier for the queue (mapped to a port).
     * @param mode Whether this instance sends or receives messages.
     */
    MessageQueue(const std::string& name, Mode mode);
    ~MessageQueue();

    /**
     * @brief Sends a message buffer.
     * @param buffer The data to send.
     * @return true on success, false otherwise.
     */
    bool send(const std::vector<uint8_t>& buffer);

    /**
     * @brief Receives a message buffer. Blocks until a message is available.
     * @param buffer The vector to store received data.
     * @return true on success, false otherwise.
     */
    bool receive(std::vector<uint8_t>& buffer);

private:
    std::string name_;
    Mode mode_;
    int sock_fd_;
    int client_fd_;
    uint16_t port_;

    uint16_t name_to_port(const std::string& name);
    bool init_sender();
    bool init_receiver();
};

} // namespace ipc
} // namespace common

#endif // COMMON_IPC_MQ_WRAPPER_H
