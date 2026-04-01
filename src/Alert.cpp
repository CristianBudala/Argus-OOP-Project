#include "../include/Alert.h"
#include <chrono>

Alert::Alert(const std::string& machineName, const std::string& metricName, double value, double threshold, std::chrono::system_clock::time_point timestamp)
    : machineName(machineName), metricName(metricName), value(value), threshold(threshold), timestamp(timestamp) {}

void Alert::displayAlert() const {
    auto time = std::chrono::system_clock::to_time_t(timestamp);
    std::put_time(std::localtime(&time), "%H:%M:%S");
}