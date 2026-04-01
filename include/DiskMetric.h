#pragma once
#include "HardwareMetric.h"

class DiskMetric : public HardwareMetric {
public:
    explicit DiskMetric(const std::string& name, double maxValue);
    void collect() override; // suprascriem metoda virtuala din clasa HardwareMetric
};