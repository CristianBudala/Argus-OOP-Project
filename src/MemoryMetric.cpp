#include "../include/MemoryMetric.h"
#include <random>

MemoryMetric::MemoryMetric(const std::string& name, double maxValue)
    : Metric(name, "MB"), HardwareMetric(name, "MB", maxValue) {}

void MemoryMetric::collect() {
    auto gen = std::mt19937(std::random_device{}());
    value = std::uniform_real_distribution<double>{0, maxValue}(gen);
}