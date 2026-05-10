#pragma once
#include <string>
#include "Alert.h"

class IMetricObserver {
public:
    virtual ~IMetricObserver() = default;
    virtual void onThresholdExceeded(const Alert& alert) = 0;
};