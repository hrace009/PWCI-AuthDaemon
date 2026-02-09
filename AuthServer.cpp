#include "AuthServer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>

#include "AuthClient.h"
#include "Settings.h"
#include "Utils.h"

AuthServer *AuthServer::self = nullptr;

AuthServer *AuthServer::Instance() {
    if (!self) {
        self = new AuthServer();
    }
    return self;
}

void AuthServer::Start() {
    if (running_) {
        return;
    }

    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        throw std::runtime_error("Failed to create server socket");
    }

    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(Settings::auth_port));
    if (inet_pton(AF_INET, Settings::auth_host.c_str(), &addr.sin_addr) != 1) {
        close(server_fd_);
        throw std::runtime_error("Invalid listen address: " + Settings::auth_host);
    }

    if (bind(server_fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        close(server_fd_);
        throw std::runtime_error("Failed to bind server socket");
    }

    if (listen(server_fd_, 128) < 0) {
        close(server_fd_);
        throw std::runtime_error("Failed to listen on server socket");
    }

    running_ = true;
    Utils::print("Listening on " + Settings::auth_host + ":" + std::to_string(Settings::auth_port));

    accept_thread_ = std::thread(&AuthServer::acceptLoop, this);

    if (Settings::antibrut_enabled) {
        brute_thread_ = std::thread(&AuthServer::bruteCleanerLoop, this);
    }
}

void AuthServer::Stop() {
    running_ = false;
    if (server_fd_ >= 0) {
        close(server_fd_);
        server_fd_ = -1;
    }
    if (accept_thread_.joinable()) {
        accept_thread_.join();
    }
    if (brute_thread_.joinable()) {
        brute_thread_.join();
    }
}

void AuthServer::acceptLoop() {
    while (running_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd_, reinterpret_cast<sockaddr *>(&client_addr), &client_len);
        if (client_fd < 0) {
            if (running_) {
                Utils::print("Accept failed");
            }
            continue;
        }

        char ip_buffer[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET, &client_addr.sin_addr, ip_buffer, sizeof(ip_buffer));
        std::string client_ip = ip_buffer;
        int client_port = ntohs(client_addr.sin_port);

        if (bruteForceManager(client_ip)) {
            Utils::print("Blocked connection from " + client_ip);
            close(client_fd);
            continue;
        }

        auto *client = new AuthClient(client_fd, client_ip, client_port);
        client->start();
    }
}

bool AuthServer::bruteForceManager(const std::string &ip) {
    if (!Settings::antibrut_enabled) {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    auto now = std::chrono::system_clock::now();

    auto blocked_it = blocked_.find(ip);
    if (blocked_it != blocked_.end()) {
        auto unblock_time = blocked_it->second + std::chrono::seconds(Settings::antibrut_blocktime);
        if (now > unblock_time) {
            blocked_.erase(blocked_it);
            Utils::print("Ip: " + ip + " has been unblocked");
            return false;
        }
        return true;
    }

    unsigned int count = ++challenges_[ip];
    if (count > static_cast<unsigned int>(Settings::antibrut_count)) {
        blocked_[ip] = now;
        Utils::print("Ip: " + ip + " has been blocked");
        return true;
    }

    return false;
}

void AuthServer::bruteCleanerLoop() {
    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(Settings::antibrut_interval));
        std::lock_guard<std::mutex> lock(mutex_);
        challenges_.clear();
    }
}
