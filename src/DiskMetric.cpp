#include "../include/DiskMetric.h"
#include <random>

DiskMetric::DiskMetric(const std::string& name, double maxValue, double threshold)
    : Metric(name, "GB"), HardwareMetric(name, "GB", maxValue), ThresholdMetric(name, "GB", threshold) {}

void DiskMetric::collect() {
    auto gen = std::mt19937(std::random_device{}());
    value = std::uniform_real_distribution<double>{0, maxValue}(gen);
    lastCollected = std::chrono::system_clock::now();
}

bool DiskMetric::isExceeded() const {
    return getUsagePercent() > threshold;
}