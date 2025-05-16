#include "implant_manager.h"

void ImplantManager::update_implant_state(const std::string& agent_id, bool active, int battery_level) {
    std::lock_guard<std::mutex> lock(mtx_);
    implants_[agent_id] = ImplantState{agent_id, active, battery_level};
}

ImplantState ImplantManager::get_implant_state(const std::string& agent_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = implants_.find(agent_id);
    if (it != implants_.end())
        return it->second;
    else
        return ImplantState{agent_id, false, 0};
}
