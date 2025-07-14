#include "utils/Logger.h"

LogLevel Logger::_log_level = LogLevel::INFO;
bool Logger::_verbose_logging = false;
std::mutex Logger::_mutex;
