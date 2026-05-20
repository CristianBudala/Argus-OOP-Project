#include "../include/ConfigManager.h"
#include "../include/MetricFactory.h"
#include "../include/ThresholdMetric.h"
#include "../include/HardwareMetric.h"
#include "../include/json.hpp"
#include <fstream>
#include <iostream>

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
        machine["metrics"] = json::array();

        for (const auto& metric : m->getMetrics()) {
            json met;
            met["type"] = metric->getType();
            met["name"] = metric->getName();

            ThresholdMetric* tm = dynamic_cast<ThresholdMetric*>(metric);
            if (tm)
                met["threshold"] = tm->getThreshold();
            
            HardwareMetric* hm = dynamic_cast<HardwareMetric*>(metric);
            if (hm)
                met["maxValue"] = hm->getMaxValue();
            
            machine["metrics"].push_back(met);
        }
        j["machines"].push_back(machine);
    }

    j["alerts"] = json::array();
    for (const auto& a : alerts) {
        json alert;
        alert["machine"] = a.getMachineName();
        alert["metric"] = a.getMetricName();
        alert["value"] = a.getValue();
        alert["threshold"] = a.getThreshold();
        alert["usagePercent"] = a.getUsagePercent();
        alert["unit"] = a.getUnit();
        j["alerts"].push_back(alert);
    }

    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + filename);
    file << j.dump(4); // indentare cu 4 spatii
    file.close();
}

void ConfigManager::load(std::vector<Machine*>& machines, 
                    std::vector<Alert>& alerts, 
                    const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + filename);
    
    json j = json::parse(file);

    for (const auto& m : j["machines"]) {
        std::string hostname = m["hostname"];
        std::string ip = m["ip"];
        Machine* machine = new Machine(hostname, ip);
        
        for (const auto& met : m["metrics"]) {
            std::string type = met["type"];
            std::string name = met["name"];
            double threshold = met.value("threshold", 0.0);
            double maxValue = met.value("maxValue", 0.0);

            Metric* metric = MetricFactory::create(type, name, maxValue, threshold);
            machine->addMetric(metric);
        }
        machines.push_back(machine);
    }

    for (const auto& a : j["alerts"]) {
        std::string machine = a["machine"];
        std::string metric = a["metric"];
        double value = a["value"];
        double threshold = a["threshold"];
        double usagePercent = a["usagePercent"];
        std::string unit = a["unit"];
        Alert alert(machine, metric, value, threshold, std::chrono::system_clock::now(), unit, usagePercent);
        alerts.push_back(alert); 
    }
}