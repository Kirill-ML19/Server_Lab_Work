#include "utils.h"

#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <filesystem>

namespace {
    std::mutex log_mtx;
    std::ofstream log_file;
    const std::string log_dir = "logs";
    const std::string log_file_path = log_dir + "/server.log";

    std::string log_level_to_string(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::DEBUG: return "DEBUG";
            default: return "UNKNOWN";
        }
    }
}

void init_logs() {
    std::lock_guard<std::mutex> lock(log_mtx);
    std::filesystem::create_directory(log_dir);
    log_file.open(log_file_path, std::ios::app);
}

void close_logs() {
    std::lock_guard<std::mutex> lock(log_mtx);
    if (log_file.is_open()) {
        log_file.close();
    }
}

void log(const std::string& msg, LogLevel level) {
    std::lock_guard
