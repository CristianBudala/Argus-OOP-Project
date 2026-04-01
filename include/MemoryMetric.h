#pragma once
#include "HardwareMetric.h"

class MemoryMetric : public HardwareMetric {
public:
    explicit MemoryMetric(const std::string& name, double maxValue);
    void collect() override; // suprascriem metoda virtuala din clasa HardwareMetric
};