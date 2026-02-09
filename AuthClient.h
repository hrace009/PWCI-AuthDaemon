#ifndef AUTHCLIENT_H
#define AUTHCLIENT_H

#include <string>
#include <thread>

class AuthClient {
public:
    AuthClient(int socket_fd, std::string remote_ip, int remote_port);
    void start();

private:
    void run();

    int socket_fd_;
    std::string remote_ip_;
    int remote_port_;
    std::thread worker_;
};

#endif // AUTHCLIENT_H
