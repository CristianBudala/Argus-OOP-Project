#include "../include/ThresholdMetric.h"

ThresholdMetric::ThresholdMetric(const std::string& name, const std::string& unit, double threshold)
    : Metric(name, unit), threshold(threshold) {}

bool ThresholdMetric::isExceeded() const { return value > threshold; }

double ThresholdMetric::getThreshold() const { return threshold; }