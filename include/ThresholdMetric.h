#pragma once
#include "Metric.h"

class ThresholdMetric : virtual public Metric {
protected:
    double threshold;

public:
    ThresholdMetric(const std::string& name, const std::string& unit, double threshold);   
    
    
    bool isExceeded() const; // true daca value > threshold
    double getThreshold() const; // Getter

    virtual void collect() = 0;
};