#include "../include/CPUMetric.h"
#include <random>

CPUMetric::CPUMetric(const std::string& name, double threshold) 
    : Metric(name, "%"), HardwareMetric(name, "%", 100.00), ThresholdMetric(name, "%", threshold) {}
// pentru ca in HardwareMetric am mostenit virtual constructorul Metric,
// constructorul clasei de baza virtuale trebuie apelat direct de clasa cea mai
// derivata -  NU PRIN INTERMEDIARI

void CPUMetric::collect() { 
    auto gen = std::mt19937(std::random_device{}());
    value = (std::uniform_real_distribution<double>{0.0, 100.0}(gen)); 
    lastCollected = std::chrono::system_clock::now();
} 
// 0.0 - 99.9 (valoare aleatoare de CPU Usage, momentan e complicat de extras din sistem)