#include "AuthClient.h"

#include <array>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include "Utils.h"

AuthClient::AuthClient(int socket_fd, std::string remote_ip, int remote_port)
    : socket_fd_(socket_fd), remote_ip_(std::move(remote_ip)), remote_port_(remote_port) {}

void AuthClient::start() {
    worker_ = std::thread(&AuthClient::run, this);
    worker_.detach();
}

void AuthClient::run() {
    Utils::print("Client connected from " + remote_ip_ + ":" + std::to_string(remote_port_));

    std::array<char, 4096> buffer{};
    while (true) {
        ssize_t bytes = recv(socket_fd_, buffer.data(), buffer.size(), 0);
        if (bytes == 0) {
            Utils::print("Client disconnected from " + remote_ip_ + ":" + std::to_string(remote_port_));
            break;
        }
        if (bytes < 0) {
            Utils::print("Socket error from " + remote_ip_ + ":" + std::string(strerror(errno)));
            break;
        }
        Utils::print("Received " + std::to_string(bytes) + " bytes from " + remote_ip_);
    }

    close(socket_fd_);
}
