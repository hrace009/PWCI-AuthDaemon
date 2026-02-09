#include "Utils.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string Utils::nowTime() {
    using clock = std::chrono::system_clock;
    auto now = clock::now();
    std::time_t time = clock::to_time_t(now);
    std::tm local_time{};
#ifdef _WIN32
    localtime_s(&local_time, &time);
#else
    localtime_r(&time, &local_time);
#endif
    std::ostringstream stream;
    stream << std::put_time(&local_time, "%H:%M:%S");
    return stream.str();
}

void Utils::print(const std::string &text) {
    std::cout << "[AuthServer][" << nowTime() << "] " << text << std::endl;
}
