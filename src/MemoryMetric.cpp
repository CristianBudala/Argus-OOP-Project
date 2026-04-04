#include "../include/MemoryMetric.h"
#include <random>

MemoryMetric::MemoryMetric(const std::string& name, double maxValue, double threshold)
    : Metric(name, "MB"), HardwareMetric(name, "MB", maxValue), ThresholdMetric(name, "MB", threshold) {}

void MemoryMetric::collect() {
    auto gen = std::mt19937(std::random_device{}());
    value = std::uniform_real_distribution<double>{0, maxValue}(gen);
    lastCollected = std::chrono::system_clock::now();
}

bool MemoryMetric::isExceeded() const {
    return getUsagePercent() > threshold;
}