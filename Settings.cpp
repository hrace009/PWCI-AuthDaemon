#include "Settings.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

std::string Settings::auth_host = "";
int Settings::auth_port = 0;
int Settings::gold_timeout = 0;
bool Settings::byLogin = false;
bool Settings::byEmail = false;
bool Settings::byToken = false;
int Settings::hash_type = 0;
std::string Settings::mysql_host = "";
int Settings::mysql_port = 0;
std::string Settings::mysql_user = "";
std::string Settings::mysql_pass = "";
std::string Settings::mysql_db = "";
bool Settings::validator_enabled = false;
std::string Settings::validator_string = "";
bool Settings::antibrut_enabled = false;
int Settings::antibrut_count = 0;
int Settings::antibrut_interval = 0;
int Settings::antibrut_blocktime = 0;

namespace {
std::string trim(std::string value) {
    auto not_space = [](unsigned char ch) { return !std::isspace(ch); };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), not_space));
    value.erase(std::find_if(value.rbegin(), value.rend(), not_space).base(), value.end());
    return value;
}

bool parseBool(const std::string &value) {
    std::string lower = value;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char ch) { return std::tolower(ch); });
    return lower == "1" || lower == "true" || lower == "yes" || lower == "on";
}

std::map<std::string, std::map<std::string, std::string>> loadIni(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Unable to open config file: " + path);
    }

    std::map<std::string, std::map<std::string, std::string>> data;
    std::string line;
    std::string section;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#') {
            continue;
        }
        if (line.front() == '[' && line.back() == ']') {
            section = trim(line.substr(1, line.size() - 2));
            continue;
        }
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }
        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));
        data[section][key] = value;
    }
    return data;
}
} // namespace

void Settings::Init(const std::string &path) {
    auto config = loadIni(path);

    auth_host = config["GAuthServer"]["host"];
    auth_port = std::stoi(config["GAuthServer"]["port"]);
    std::string auth_type = config["GAuthServer"]["allow_auth_type"];
    byLogin = auth_type.find("login") != std::string::npos;
    byEmail = auth_type.find("email") != std::string::npos;
    byToken = auth_type.find("token") != std::string::npos;
    gold_timeout = std::stoi(config["GAuthServer"]["gold_timeout"]);
    hash_type = std::stoi(config["GAuthServer"]["hash_type"]);

    mysql_host = config["Mysql"]["host"];
    mysql_port = std::stoi(config["Mysql"]["port"]);
    mysql_user = config["Mysql"]["user"];
    mysql_pass = config["Mysql"]["pass"];
    mysql_db = config["Mysql"]["db"];

    validator_enabled = parseBool(config["Validator"]["is_enabled"]);
    validator_string = config["Validator"]["regex"];

    antibrut_enabled = parseBool(config["Antibrut"]["is_enabled"]);
    antibrut_count = std::stoi(config["Antibrut"]["count"]);
    antibrut_interval = std::stoi(config["Antibrut"]["interval"]);
    antibrut_blocktime = std::stoi(config["Antibrut"]["block_time"]);
}
