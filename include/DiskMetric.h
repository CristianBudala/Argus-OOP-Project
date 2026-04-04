#pragma once
#include "HardwareMetric.h"
#include "ThresholdMetric.h"

class DiskMetric : public HardwareMetric, public ThresholdMetric {
public:
    explicit DiskMetric(const std::string& name, double maxValue, double threshold);
    void collect() override; // suprascriem metoda virtuala din clasa HardwareMetric
    virtual bool isExceeded() const;
};