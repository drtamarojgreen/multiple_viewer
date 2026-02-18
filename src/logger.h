#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

enum class LogLevel { INFO, WARNING, ERROR, DEBUG };

class Logger {
public:
    static void log(LogLevel level, const std::string& message) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::cout << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << "] ";

        switch (level) {
            case LogLevel::INFO:    std::cout << "[INFO] "; break;
            case LogLevel::WARNING: std::cout << "[WARNING] "; break;
            case LogLevel::ERROR:   std::cout << "[ERROR] "; break;
            case LogLevel::DEBUG:   std::cout << "[DEBUG] "; break;
        }

        std::cout << message << std::endl;
    }

    static void info(const std::string& msg) { log(LogLevel::INFO, msg); }
    static void warn(const std::string& msg) { log(LogLevel::WARNING, msg); }
    static void error(const std::string& msg) { log(LogLevel::ERROR, msg); }
    static void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
};

#endif // LOGGER_H
