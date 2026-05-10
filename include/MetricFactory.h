#pragma once
#include "Metric.h"

class MetricFactory{
public:
    MetricFactory() = delete;

    static Metric* create(const std::string& type,
                            const std::string& name,
                            double param1 = 0.0, double param2 = 0.0);
    // param1 & param2 default parameters: NetworkMetric does not need parameters,
    // CPUMetric needs one and MemoryMetric needs 2
};