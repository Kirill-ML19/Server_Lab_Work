#pragma once

#include <string>
#include <unordered_map>

struct Message {
    std::string command;
    std::unordered_map<std::string, std::string> params;
};

class Protocol {
public:
    static Message parse_message(const std::string& raw);
    static std::string serialize_response(const std::string& status, const std::string& info = "");
};
