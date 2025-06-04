#include "utils.h"

#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
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

    std::string current_time_string() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm;
#ifdef _WIN32
        localtime_s(&local_tm, &now_time_t);
#else
        localtime_r(&now_time_t, &local_tm);
#endif
        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
}

void init_logs() {
    std::lock_guard<std::mutex> lock(log_mtx);
    std::error_code ec;
    std::filesystem::create_directory(log_dir, ec);
    if (ec) {
        std::cerr << "Failed to create log directory: " << ec.message() << std::endl;
    }
    log_file.open(log_file_path, std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Failed to open log file: " << log_file_path << std::endl;
    }
}

void close_logs() {
    std::lock_guard<std::mutex> lock(log_mtx);
    if (log_file.is_open()) {
        log_file.close();
    }
}

void log(const std::string& msg, LogLevel level) {
    std::lock_guard<std::mutex> lock(log_mtx);
    std::string time_str = current_time_string();
    std::string level_str = log_level_to_string(level);

    std::string final_msg = "[" + time_str + "] [" + level_str + "] " + msg;

    if (log_file.is_open()) {
        log_file << final_msg << std::endl;
        log_file.flush();
    }

    if (level == LogLevel::ERROR) {
        std::cerr << final_msg << std::endl;
    } else {
        std::cout << final_msg << std::endl;
    }
}
