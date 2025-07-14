#pragma once

#include <string>
#include <iostream>
#include <mutex>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static void set_log_level(LogLevel level) {
        _log_level = level;
    }

    static void set_verbose_logging(bool verbose) {
        _verbose_logging = verbose;
    }

    static void debug(const std::string& message) {
        log(LogLevel::DEBUG, "DEBUG", message);
    }

    static void info(const std::string& message) {
        log(LogLevel::INFO, "INFO", message);
    }

    static void warn(const std::string& message) {
        log(LogLevel::WARN, "WARNING", message);
    }

    static void error(const std::string& message) {
        log(LogLevel::ERROR, "ERROR", message);
    }

private:
    static void log(LogLevel level, const std::string& prefix, const std::string& message) {
        if (!_verbose_logging && level == LogLevel::DEBUG) {
            return;
        }
        if (level >= _log_level) {
            std::lock_guard<std::mutex> lock(_mutex);
            std::cerr << "[" << prefix << "] " << message << std::endl;
        }
    }

    static LogLevel _log_level;
    static bool _verbose_logging;
    static std::mutex _mutex;
};

// Initialize static members
// These will be defined in the .cpp file
// LogLevel Logger::_log_level = LogLevel::INFO;
// bool Logger::_verbose_logging = false;
// std::mutex Logger::_mutex;
