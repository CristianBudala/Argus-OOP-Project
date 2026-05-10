#pragma once
#include "Machine.h"
#include "Alert.h"
#include <vector>
#include <string>

class ConfigManager {
private:

public:
    ConfigManager() = delete;

    static void save(const std::vector<Machine*>& machines, 
                    const std::vector<Alert>& alerts, 
                    const std::string& filename);

    static void load(std::vector<Machine*>& machines, 
                    std::vector<Alert>& alerts, 
                    const std::string& filename);
};