#include "client_session.h"
#include "protocol.h"
#include "utils.h"

#include <unistd.h>
#include <iostream>
#include <string>

ClientSession::ClientSession(int client_fd, ImplantManager& implant_manager)
    : client_fd_(client_fd), implant_manager_(implant_manager) {}

void ClientSession::run() {
    handle_client();
}

void ClientSession::handle_client() {
    const int buf_size = 1024;
    char buffer[buf_size];
    std::string read_buffer;

    while (true) {
        ssize_t bytes_read = read(client_fd_, buffer, buf_size - 1);
        if (bytes_read == 0) {
            log("Client disconnected", LogLevel::INFO);
            break;
        } else if (bytes_read < 0) {
            log("Read error from client", LogLevel::ERROR);
            break;
        }

        buffer[bytes_read] = '\0';
        read_buffer.append(buffer);

        size_t pos;
        while ((pos = read_buffer.find('\n')) != std::string::npos) {
            std::string line = read_buffer.substr(0, pos);
            read_buffer.erase(0, pos + 1);

            if (line.empty()) continue;

            Message msg;
            try {
                msg = Protocol::parse_message(line);
            } catch (...) {
                log("Failed to parse message: " + line, LogLevel::WARNING);
                std::string err_resp = Protocol::serialize_response("ERROR", "Invalid message format") + "\n";
                if (write(client_fd_, err_resp.c_str(), err_resp.size()) <= 0) {
                    log("Write error", LogLevel::ERROR);
                    return;
                }
                continue;
            }

            std::string response;

            if (msg.command == "UPDATE_STATE") {
                auto it_id = msg.params.find("agent_id");
                auto it_active = msg.params.find("active");
                auto it_battery = msg.params.find("battery");

                if (it_id != msg.params.end() && it_active != msg.params.end() && it_battery != msg.params.end()) {
                    std::string agent_id = it_id->second;
                    bool active = (it_active->second == "1");
                    int battery = 0;
                    try {
                        battery = std::stoi(it_battery->second);
                    } catch (...) {
                        response = Protocol::serialize_response("ERROR", "Invalid battery level");
                        log("Invalid battery level received", LogLevel::WARNING);
                        goto send_response;
                    }

                    implant_manager_.update_implant_state(agent_id, active, battery);

                    log("Updated state: agent_id=" + agent_id +
                        " active=" + (active ? "1" : "0") +
                        " battery=" + std::to_string(battery), LogLevel::INFO);

                    response = Protocol::serialize_response("OK", "State updated");
                } else {
                    log("UPDATE_STATE error: missing parameters", LogLevel::WARNING);
                    response = Protocol::serialize_response("ERROR", "Missing parameters");
                }

            } else if (msg.command == "GET_STATE") {
                auto it_id = msg.params.find("agent_id");
                if (it_id != msg.params.end()) {
                    ImplantState state = implant_manager_.get_implant_state(it_id->second);
                    std::string info = "agent_id:" + state.agent_id +
                                       " active:" + (state.active ? "1" : "0") +
                                       " battery:" + std::to_string(state.battery_level);

                    log("Sent state to client: " + info, LogLevel::INFO);

                    response = Protocol::serialize_response("OK", info);
                } else {
                    log("GET_STATE error: missing agent_id", LogLevel::WARNING);
                    response = Protocol::serialize_response("ERROR", "Missing agent_id");
                }

            } else {
                response = Protocol::serialize_response("ERROR", "Unknown command");
            }

        send_response:
            response += "\n";
            ssize_t bytes_sent = write(client_fd_, response.c_str(), response.size());
            if (bytes_sent <= 0) {
                log("Write error", LogLevel::ERROR);
                return;
            }
        }
    }
}
