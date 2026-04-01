#include "../include/Metric.h"
#include <iomanip>
#include <ctime>

Metric::Metric(const std::string& name, const std::string& unit) 
    : name(name), value(0.0), unit(unit), 
      lastCollected(std::chrono::system_clock::now()) {} 

const std::string& Metric::getName() const { return name; }
double Metric::getValue() const { return value; }
const std::string& Metric::getUnit() const { return unit; }

std::ostream& operator<<(std::ostream& os, const Metric& m) {
    auto time = std::chrono::system_clock::to_time_t(m.lastCollected);
    os << m.name << ": " << std::fixed << std::setprecision(1)
       << m.value << " " << m.unit << " (at " 
       << std::put_time(std::localtime(&time), "%H:%M:%S") << ")";
    return os; // ex. CPU_usage 47.3% (at 19:26:07): 
}

// Destructorul clasei Metric e virtual pt evitarea de memory leak.
// Avem cel putin o metoda virtuala 'collect()' -> facem si destructorul virtual

// = 0 pe collect() ca sa facem clasa "abstracta".

// friend pe operator<< deoarece nu e metoda a clasei ci o functie libera 
// care are nevoie de acces la valorile private/protejate pentru print (m.name, m.value etc.)

// value e protected nu private pt ca subclasele vor scrie in el din collect()