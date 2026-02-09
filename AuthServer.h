#ifndef AUTHSERVER_H
#define AUTHSERVER_H

#include <atomic>
#include <chrono>
#include <map>
#include <mutex>
#include <string>
#include <thread>

class AuthServer {
public:
    static AuthServer *Instance();

    void Start();
    void Stop();

private:
    AuthServer() = default;
    void acceptLoop();
    void bruteCleanerLoop();
    bool bruteForceManager(const std::string &ip);

    static AuthServer *self;

    std::atomic<bool> running_{false};
    int server_fd_ = -1;
    std::thread accept_thread_;
    std::thread brute_thread_;

    std::mutex mutex_;
    std::map<std::string, unsigned int> challenges_;
    std::map<std::string, std::chrono::system_clock::time_point> blocked_;
};

#endif // AUTHSERVER_H
