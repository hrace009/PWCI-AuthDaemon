#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings {
public:
    static void Init(const std::string &path);

    static std::string auth_host;
    static int auth_port;
    static int gold_timeout;
    static bool byLogin;
    static bool byEmail;
    static bool byToken;
    static int hash_type;

    static std::string mysql_host;
    static int mysql_port;
    static std::string mysql_user;
    static std::string mysql_pass;
    static std::string mysql_db;

    static bool validator_enabled;
    static std::string validator_string;

    static bool antibrut_enabled;
    static int antibrut_count;
    static int antibrut_interval;
    static int antibrut_blocktime;
};

#endif // SETTINGS_H
