#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <Exceptions.h>

// Generic metric class that forces any subtype to define collect()
class Metric {
protected:
    std::string name; // (ex. "CPU_usage")
    double value; // (ex. 27.3%)
    std::string unit; // (ex. "%", "MB", "MB/s")
    std::chrono::system_clock::time_point lastCollected; // timestamp

public:
    Metric(const std::string& name, const std::string& unit);
    virtual ~Metric() = default;

    // Purely virtual, each subtype decides how to collect()
    // = 0 forces every class that inherits Metric to implement its own collect()
    // without = 0, collect() would work in Metric as a void function
    virtual void collect() = 0;

    // Getters
    const std::string& getName() const;
    double getValue() const;
    const std::string& getUnit() const;

    // Print
    friend std::ostream& operator<<(std::ostream& os, const Metric& m);
};