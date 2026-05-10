#include "../include/MetricFactory.h"
#include "../include/CPUMetric.h"
#include "../include/MemoryMetric.h"
#include "../include/DiskMetric.h"
#include "../include/NetworkMetric.h"
#include "../include/Exceptions.h"

Metric* MetricFactory::create(const std::string& type,
                            const std::string& name,
                            double param1, double param2){
if (type == "cpu")      return new CPUMetric(name, param1);
if (type == "memory")   return new MemoryMetric(name, param1, param2);
if (type == "disk")     return new DiskMetric(name, param1, param2);
if (type == "network")  return new NetworkMetric(name);
throw InvalidInputException("Unknown metric type: " + type);  
}