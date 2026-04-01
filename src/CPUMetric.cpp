#include "../include/CPUMetric.h"

CPUMetric::CPUMetric(const std::string& name, double threshold) 
    : Metric(name, "%"), HardwareMetric(name, "%", 100.00), ThresholdMetric(name, "%", threshold) {}
// pentru ca in HardwareMetric am mostenit virtual constructorul Metric,
// constructorul clasei de baza virtuale trebuie apelat direct de clasa cea mai
// derivata -  NU PRIN INTERMEDIARI

void CPUMetric::collect() { value = (std::rand() % 1000) / 10.0; } 
// 0.0 - 99.9 (valoare aleatoare de CPU Usage, momentan e complicat de extras din sistem)