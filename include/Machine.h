#pragma once
#include <string>
#include <vector>
#include "Metric.h"

// Machine (un server, un laptop, un VM) e containerul care tine
// toate metricile individuale (CPUMetric, MemoryMetric etc.) impreuna

class Machine {
    public:
        Machine(const std::string& hostname, const std::string& ipAddress);
        ~Machine();

        // Copierea nu are sens pt o machina monitorizata deja
        Machine(const Machine&) = delete; // copierea e interzisa intentionat
        Machine& operator=(const Machine&) = delete; // copierea e interzisa intentionat
        
        void addMetric(Metric* m);
        void collectAll();
        void displayAll() const;
        
        const std::string& getHostname() const; // Getter
        const std::string& getIpAddress() const; // Getter
        const std::vector<Metric*>& getMetrics() const; // Getter

    private:
        std::string hostname;
        std::string ipAddress;
        std::vector<Metric*> metrics;
        // <Metric*> ca vector de pointeri deoarece:
        // 1. Metric e abstracta - are collect() = 0;. Nu pot crea obiecte de tip Metric direct, deci nu le pot nici adauga in vector
        // 2. Si daca nu ar fi abtracta, pe mine ma intereseaza sa adaug in vector subclase Metric (CPU, Memory, Disk). 
        // Fara pointer as pierde toate valorile specifice lor (maxValue, threshold etc.)
        // cu vector<Metric*> polimorfismul functioneaza corect
};