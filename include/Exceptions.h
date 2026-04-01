#pragma once
#include <exception>
#include <string>


// The base for all handled exceptions
class ArgusException : public std::exception {
protected:
    std::string message;
public:
    explicit ArgusException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); } // noexcept so that what() itself cannot throw error
};                                                                         // by the std::exception convention

// Invalid value for a metric of a certain hardware device (eg. CPU = -5%)
class InvalidMetricException : public ArgusException {
public:
    explicit InvalidMetricException(const std::string& metricName, double value)
        : ArgusException("Invalid value for " + metricName + ": " + std::to_string(value)) {}
};

// Threshold exceeded (eg. CPU > 95%)
class ThresholdExceededException : public ArgusException {
public:
    explicit ThresholdExceededException(const std::string& metricName, 
                                        double value, double threshold)
        : ArgusException("Threshold exceeded for " + metricName + ": " + std::to_string(value) 
                            + " > " + std::to_string(threshold)) {}
};

// Machine not found by IP/hostname
class MachineNotFoundException : public ArgusException {
public:
    explicit MachineNotFoundException(const std::string& identifier) 
        : ArgusException("Machine not found: " + identifier) {}
};

class InvalidInputException : public ArgusException {
public:
    explicit InvalidInputException(const std::string& details)
        : ArgusException("Invalid input: " + details) {}
};