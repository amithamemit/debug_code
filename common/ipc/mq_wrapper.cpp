#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #ifndef INVALID_SOCKET
        #define INVALID_SOCKET -1
    #endif
    #ifndef SOCKET_ERROR
        #define SOCKET_ERROR -1
    #endif
    typedef int SOCKET;
    #define closesocket close
#endif

#include "mq_wrapper.h"
#include <iostream>
#include <cstring>

namespace common {
namespace ipc {

MessageQueue::MessageQueue(const std::string& name, Mode mode) 
    : name_(name), mode_(mode), sock_fd_((int)INVALID_SOCKET), client_fd_((int)INVALID_SOCKET) {
    
    port_ = name_to_port(name);

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    if (mode == Mode::SENDER) {
        init_sender();
    } else {
        init_receiver();
    }
}

MessageQueue::~MessageQueue() {
    if ((SOCKET)client_fd_ != INVALID_SOCKET) closesocket(client_fd_);
    if ((SOCKET)sock_fd_ != INVALID_SOCKET) closesocket(sock_fd_);
#ifdef _WIN32
    WSACleanup();
#endif
}

uint16_t MessageQueue::name_to_port(const std::string& name) {
    // Simple hash to map a name to a port in a "safe" range (5000-6000)
    uint16_t hash = 0;
    for (char c : name) hash = (hash * 31) + c;
    return 5000 + (hash % 1000);
}

bool MessageQueue::init_sender() {
    sock_fd_ = (int)socket(AF_INET, SOCK_STREAM, 0);
    if ((SOCKET)sock_fd_ == INVALID_SOCKET) return false;

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_);
#ifdef _WIN32
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
#else
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
#endif

    // Keep trying to connect (sender waits for receiver/server)
    while (connect(sock_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
#ifdef _WIN32
        Sleep(100);
#else
        usleep(1000000);
#endif
    }
    return true;
}

bool MessageQueue::init_receiver() {
    sock_fd_ = (int)socket(AF_INET, SOCK_STREAM, 0);
    if ((SOCKET)sock_fd_ == INVALID_SOCKET) return false;

    int opt = 1;
    setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(sock_fd_, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) return false;
    if (listen(sock_fd_, 3) == SOCKET_ERROR) return false;

    // We no longer accept here; we defer it to receive() to allow re-accepting
    return true;
}

bool MessageQueue::send(const std::vector<uint8_t>& buffer) {
    uint32_t size = static_cast<uint32_t>(buffer.size());
    
    // For SENDER, if we lost connection, try to reconnect
    if (mode_ == Mode::SENDER && (SOCKET)sock_fd_ == INVALID_SOCKET) {
        std::cout << "[MQ] Attempting to reconnect sender for '" << name_ << "'..." << std::endl;
        if (!init_sender()) return false;
    }

    SOCKET target = (mode_ == Mode::SENDER) ? sock_fd_ : client_fd_;
    if (target == (SOCKET)INVALID_SOCKET) return false;

    // Send size first
    if (::send(target, (const char*)&size, sizeof(size), 0) <= 0) {
        if (mode_ == Mode::SENDER) {
            closesocket(sock_fd_);
            sock_fd_ = (int)INVALID_SOCKET;
        } else {
            closesocket(client_fd_);
            client_fd_ = (int)INVALID_SOCKET;
        }
        return false;
    }

    // Send data
    if (::send(target, (const char*)buffer.data(), size, 0) <= 0) {
        if (mode_ == Mode::SENDER) {
            closesocket(sock_fd_);
            sock_fd_ = (int)INVALID_SOCKET;
        } else {
            closesocket(client_fd_);
            client_fd_ = (int)INVALID_SOCKET;
        }
        return false;
    }
    
    return true;
}

bool MessageQueue::receive(std::vector<uint8_t>& buffer) {
    uint32_t size = 0;
    
    while (true) {
        SOCKET target = (mode_ == Mode::RECEIVER) ? client_fd_ : sock_fd_;
        
        if (target == (SOCKET)INVALID_SOCKET) {
            if (mode_ == Mode::SENDER) return false;
            // Receiver: try to accept a new connection
            struct sockaddr_in address;
            socklen_t addrlen = sizeof(address);
            client_fd_ = (int)accept((SOCKET)sock_fd_, (struct sockaddr*)&address, &addrlen);
            if ((SOCKET)client_fd_ == INVALID_SOCKET) return false;
            target = (SOCKET)client_fd_;
        }

        // Receive size first
        int valread = ::recv(target, (char*)&size, sizeof(size), 0);
        if (valread <= 0) {
            // Connection lost
            closesocket(target);
            if (mode_ == Mode::RECEIVER) {
                client_fd_ = (int)INVALID_SOCKET;
                std::cout << "[MQ] Client disconnected from '" << name_ << "'. Waiting for next client..." << std::endl;
                continue; // Loop to accept again
            } else {
                sock_fd_ = (int)INVALID_SOCKET;
                return false;
            }
        }

        buffer.resize(size);
        uint32_t total_received = 0;
        bool error = false;
        while (total_received < size) {
            valread = ::recv(target, (char*)buffer.data() + total_received, size - total_received, 0);
            if (valread <= 0) {
                error = true;
                break;
            }
            total_received += valread;
        }
        
        if (error) {
            closesocket(target);
            if (mode_ == Mode::RECEIVER) {
                client_fd_ = (int)INVALID_SOCKET;
                continue;
            } else {
                sock_fd_ = (int)INVALID_SOCKET;
                return false;
            }
        }

        return true;
    }
}

} // namespace ipc
} // namespace common
