#pragma once
#include "HardwareMetric.h"
#include "ThresholdMetric.h"

class CPUMetric : public HardwareMetric, public ThresholdMetric {
public:
    explicit CPUMetric(const std::string& name, double threshold);
    void collect() override; // suprascriem metoda virtuala din clasa HardwareMetric
};