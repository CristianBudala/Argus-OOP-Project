#pragma once
#include "IMetricObserver.h"
#include "Alert.h"
#include <vector>

class AlertObserver : public IMetricObserver {
private:
    std::vector<Alert>& alerts;

public:
    AlertObserver(std::vector<Alert>& alerts) : alerts(alerts) {}

    void onThresholdExceeded(const Alert& alert) override;
};