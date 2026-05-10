#include "../include/AlertObserver.h"

void AlertObserver::onThresholdExceeded(const Alert& alert) {
    alerts.push_back(alert);
    Logger::info("Alert generated for: " + alert.getMetricName());
}