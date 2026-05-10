#include "../include/ConfigManager.h"
#include "../include/json.hpp"

using json = nlohmann::json;

void ConfigManager::save(const std::vector<Machine*>& machines, 
                    const std::vector<Alert>& alerts, 
                    const std::string& filename) {
    json j;
    j["machines"] = json::array();
    for (const auto& m : machines) {
        json machine;
        machine["hostname"] = m->getHostname();
        machine["ip"] = m->getIpAddress();
        j["machines"].push_back(machine);
    }


}