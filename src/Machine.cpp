#include "../include/Machine.h"

Machine::Machine(const std::string& hostname, const std::string& ipAddress)
    : hostname(hostname), ipAddress(ipAddress) {}

Machine::~Machine() {
    for (auto mt : metrics)
        delete mt;
}

void Machine::addMetric(Metric* m) { metrics.push_back(m); }

void Machine::collectAll() { 
    for (auto mt : metrics)
        mt->collect();
}

void Machine::displayAll() const {
    for (auto mt : metrics)
        std::cout << *mt << std::endl;
}

const std::string& Machine::getHostname() const { return hostname; }

const std::string& Machine::getIpAddress() const { return ipAddress; }

const std::vector<Metric*>& Machine::getMetrics() const { return metrics; }