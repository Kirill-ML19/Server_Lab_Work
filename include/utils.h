#pragma once

#include <string>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

void log(const std::string& msg, LogLevel level=LogLevel::INFO);

void init_logs();
void close_logs();
