#pragma once
#include "HardwareMetric.h"
#include "ThresholdMetric.h"

class MemoryMetric : public HardwareMetric, public ThresholdMetric {
public:
    explicit MemoryMetric(const std::string& name, double maxValue, double threshold);
    void collect() override; // suprascriem metoda virtuala din clasa HardwareMetric
    bool isExceeded() const override;
};