// Vom avea HarwareMetric si NetworkMetric diferite deoarece una masoara
// o resursa locala finita a masinii (are mereu o valoare maxima iar utilizarea
// reprezinta un procent din aceasta). Network masoara un flux de bytes/secunda,
// nu are maxim

#pragma once
#include "Metric.h"

class HardwareMetric : virtual public Metric { // ex. CPUMetric va mosteni HardwareMetric si ThresholdMetric 
                                               // -> doua copii ale lui Metric. Cu virtual o singura copie
protected:
    double maxValue;

public:
    HardwareMetric(const std::string& name, const std::string& unit, double maxValue);
    virtual ~HardwareMetric() = default;

    double getUsagePercent() const; // (value / maxValue) * 100
    virtual void collect() = 0; // abstract - decid subclasele cum colecteaza
};