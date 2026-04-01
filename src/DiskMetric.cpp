#include "../include/DiskMetric.h"
#include <random>

DiskMetric::DiskMetric(const std::string& name, double maxValue)
    : Metric(name, "GB"), HardwareMetric(name, "GB", maxValue) {}

void DiskMetric::collect() {
    auto gen = std::mt19937(std::random_device{}());
    value = std::uniform_real_distribution<double>{0, maxValue}(gen);
}