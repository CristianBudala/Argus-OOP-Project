#pragma once
#include "Metric.h"

class NetworkMetric : public Metric {
public:
    NetworkMetric(const std::string& name);
    virtual ~NetworkMetric() = default;

    void collect() override;
};