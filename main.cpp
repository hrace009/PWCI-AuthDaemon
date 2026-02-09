#include <atomic>
#include <csignal>
#include <exception>
#include <iostream>
#include <thread>

#include "AuthServer.h"
#include "Database.h"
#include "Settings.h"
#include "Utils.h"

namespace {
std::atomic<bool> running{true};

void handleSignal(int) {
    running = false;
}
} // namespace

int main(int argc, char *argv[]) {
    const std::string config_path = (argc > 1) ? argv[1] : "gauthd.conf";

    try {
        Utils::print("GAuthDaemon (C++/MariaDB) by hrace009");
        Utils::print("Loading config: " + config_path);

        Settings::Init(config_path);
        Database::Instance()->connect();

        std::signal(SIGINT, handleSignal);
        std::signal(SIGTERM, handleSignal);

        AuthServer::Instance()->Start();

        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        AuthServer::Instance()->Stop();
        Database::Instance()->disconnect();
    } catch (const std::exception &ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
