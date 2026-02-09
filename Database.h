#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>

class Database {
public:
    static Database *Instance();

    void connect();
    void disconnect();
    bool isConnected() const;
    MYSQL *handle();

private:
    Database() = default;
    ~Database();

    static Database *self;
    MYSQL *connection = nullptr;
};

#endif // DATABASE_H
