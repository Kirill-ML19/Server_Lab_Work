#include "protocol.h"
#include <sstream>
#include <stdexcept>

Message Protocol::parse_message(const std::string& raw) {
    Message msg;
    std::istringstream iss(raw);
    if (!(iss >> msg.command)) {
        throw std::runtime_error("Empty command");
    }

    std::string token;
    while (iss >> token) {
        auto pos = token.find('=');
        if (pos == std::string::npos) {
            throw std::runtime_error("Invalid param format");
        }
        std::string key = token.substr(0, pos);
        std::string value = token.substr(pos + 1);
        msg.params[key] = value;
    }
    return msg;
}

std::string Protocol::serialize_response(const std::string& status, const std::string& info) {
    if (info.empty())
        return status;
    else
        return status + " " + info;
}
