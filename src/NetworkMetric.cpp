#include "../include/NetworkMetric.h"
#include <random>

NetworkMetric::NetworkMetric(const std::string& name)
    : Metric(name, "MB/s") {}

void NetworkMetric::collect() {
    auto gen = std::mt19937(std::random_device{}());
    value = std::uniform_real_distribution<double>{0, 1000}(gen);
    lastCollected = std::chrono::system_clock::now();
}