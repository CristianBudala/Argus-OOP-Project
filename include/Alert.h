#pragma once
#include <string>
#include <chrono>

class Alert {
public:
    Alert(const std::string& machineName, const std::string& metricName, 
        double value, double threshold, 
        std::chrono::system_clock::time_point timestamp, 
        const std::string& unit, double usagePercent = -1.0);
    ~Alert() = default;

    void displayAlert() const;

private:
    std::string machineName;
    std::string metricName;
    double value;
    double threshold;
    std::chrono::system_clock::time_point timestamp;
    double usagePercent; // doar pt HardwareMetric, -1 pentru restul
    std::string unit;
};