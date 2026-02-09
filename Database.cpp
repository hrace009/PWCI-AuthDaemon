#include "Database.h"

#include <stdexcept>

#include "Settings.h"
#include "Utils.h"

Database *Database::self = nullptr;

Database::~Database() {
    disconnect();
}

Database *Database::Instance() {
    if (!self) {
        self = new Database();
    }
    return self;
}

void Database::connect() {
    if (connection) {
        return;
    }

    connection = mysql_init(nullptr);
    if (!connection) {
        throw std::runtime_error("Failed to initialize MariaDB connection");
    }

    unsigned int timeout = 300;
    mysql_options(connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
    mysql_options(connection, MYSQL_OPT_RECONNECT, "1");

    if (!mysql_real_connect(
            connection,
            Settings::mysql_host.c_str(),
            Settings::mysql_user.c_str(),
            Settings::mysql_pass.c_str(),
            Settings::mysql_db.c_str(),
            static_cast<unsigned int>(Settings::mysql_port),
            nullptr,
            0)) {
        std::string error = mysql_error(connection);
        disconnect();
        throw std::runtime_error("MariaDB connection failed: " + error);
    }

    Utils::print("Successfully connected to MariaDB");
}

void Database::disconnect() {
    if (connection) {
        mysql_close(connection);
        connection = nullptr;
    }
}

bool Database::isConnected() const {
    return connection != nullptr;
}

MYSQL *Database::handle() {
    return connection;
}
