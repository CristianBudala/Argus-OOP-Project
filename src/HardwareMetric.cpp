#include "../include/HardwareMetric.h"

HardwareMetric::HardwareMetric(const std::string& name, const std::string& unit, double maxValue)
    : Metric(name, unit), maxValue(maxValue) {}

double HardwareMetric::getUsagePercent() const { return (value / maxValue) * 100; }