#pragma once

#include <string>
#include <unordered_map>
#include <mutex>

struct ImplantState {
    std::string agent_id;
    bool active;
    int battery_level;
};

class ImplantManager {
public:
    ImplantManager() = default;

    void update_implant_state(const std::string& agent_id, bool active, int battery_level);
    ImplantState get_implant_state(const std::string& agent_id);

private:
    std::unordered_map<std::string, ImplantState> implants_;
    std::mutex mtx_;
};
