#include "../include/Alert.h"
#include <ctime>
#include <iomanip>
#include <iostream>

Alert::Alert(const std::string& machineName, const std::string& metricName, double value, 
    double threshold, std::chrono::system_clock::time_point timestamp, const std::string& unit, double usagePercent)
    : machineName(machineName), metricName(metricName), value(value), 
    threshold(threshold), timestamp(timestamp), usagePercent(usagePercent), unit(unit){}

void Alert::displayAlert() const {
    auto time = std::chrono::system_clock::to_time_t(timestamp);
    std::cout << "[ALERT] " << machineName << " | " << metricName
                << " : " << value << " " << unit;
    if (usagePercent >= 0)
        std::cout << " (" << std::fixed << std::setprecision(1) 
                    << usagePercent << "% > " << threshold << "%)";
    std::cout << " (at " << std::put_time(std::localtime(&time), "%H:%M:%S") << ")\n";
}

const std::string& Alert::getMetricName() const {
    return metricName;
}

const std::string& Alert::getMachineName() const {
    return machineName;
}

double Alert::getValue() const {
    return value;
}

double Alert::getThreshold() const {
    return threshold;
}

double Alert::getUsagePercent() const {
    return usagePercent;
}

const std::string& Alert::getUnit() const {
    return unit;
}

