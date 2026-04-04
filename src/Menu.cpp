#include "../include/Menu.h"
#include "../include/Logger.h"
#include "../include/CPUMetric.h"
#include "../include/MemoryMetric.h"
#include "../include/DiskMetric.h"
#include "../include/NetworkMetric.h"
#include "../include/Logger.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <limits>
#include <string>
#include <cassert>

Menu::~Menu() {
    for (auto m : machines)
        delete m;
}

void Menu::addMachine() {
    std::string hostname;
    std::string ipAddress;
    std::cout << "Hostname: ";
    std::cin >> hostname;
    std::cout << "IP Address: ";
    std::cin >> ipAddress;

    machines.push_back(new Machine(hostname, ipAddress));

    Logger::info("Machine added: " + hostname);

    std::cout << "Machine '" << hostname << "' added.\n";
}

void Menu::displayMachines() const {
    if (machines.empty()){
        std::cout << "No machines added yet.\n";
        return;
    }
    for (const auto& m : machines){
        std::cout << "[Machine] " << m->getHostname()
                 << " (" << m->getIpAddress() << ")\n";
        m->displayAll();
    }
}

void Menu::collectMetrics() {
    if (machines.empty()){
        std::cout << "No machines added yet.\n";
        return;
    }
    for (const auto& m : machines){
        m->collectAll();
        Logger::info("Metrics collected for: " + m->getHostname());
        for (const auto& metric : m->getMetrics()){
            ThresholdMetric* tm = dynamic_cast<ThresholdMetric*>(metric);
            if (tm != nullptr && tm->isExceeded()){
                Alert alert(m->getHostname(), tm->getName(), tm->getValue(), tm->getThreshold(), std::chrono::system_clock::now());
                alerts.push_back(alert);
                Logger::info("Alert generated for: " + tm->getName());
            }
        }  
    }
    std::cout << "Metrics collected for all machines.\n";
}

void Menu::addMetric() {
    if (machines.empty()){
        std::cout << "No machines added yet.\n";
        return;
    }
    size_t cnt{};
    for (const auto& m: machines){
        std::cout << ++cnt << ". " << m->getHostname() 
                    << " (" << m->getIpAddress() << ")\n";
    }
    std::cout << "Choose machine: ";
    size_t index;
    std::cin >> index;
    if (index == 0 || index > machines.size())
        throw InvalidInputException("Machine index out of range");
    Machine* machine = machines[index - 1];

    std::cout << "\nMetric type: \n";
    std::cout << "1. CPU\n";
    std::cout << "2. Memory\n";
    std::cout << "3. Disk\n";
    std::cout << "4. Network\n";
    std::cout << "Choose type: ";
    int type{};
    std::cin >> type;

    switch(type){
        case 1: {
            std::string name;
            double threshold;
            std::cout << "Name: ";
            std::cin >> name;
            std::cout << "Threshold (%): ";
            std::cin >> threshold;
            machine->addMetric(new CPUMetric(name, threshold));
            break;
        }
        case 2: {
            std::string name;
            double maxValue;
            std::cout << "Name: ";
            std::cin >> name;
            std::cout << "Maximum value: ";
            std::cin >> maxValue;
            machine->addMetric(new MemoryMetric(name, maxValue));
            break;
        }
        case 3: {
            std::string name;
            double maxValue;
            std::cout << "Name: ";
            std::cin >> name;
            std::cout << "Maximum value: ";
            std::cin >> maxValue;
            machine->addMetric(new DiskMetric(name, maxValue));
            break;
        }
        case 4: {
            std::string name;
            std::cout << "Name: ";
            std::cin >> name;
            machine->addMetric(new NetworkMetric(name));
            break;
        }
        default:
            throw InvalidInputException("Options must be between 1 and 4");
            break;
    }
}

void Menu::displayAlerts() const {
    if (alerts.empty()){
        std::cout << "No alerts recorded.\n";
        return;
    }
    for (const auto& a : alerts)
        a.displayAlert();
}

void Menu::removeMachine(){
    if(machines.empty()){
        std::cout << "No machines registered.\n";
        return;
    }
    std::cout << "Choose the machine you want to remove.\n";
    unsigned int i{};
    for (const auto& m : machines){
        std::cout << ++i << ". " << m->getHostname() << std::endl;  
    }
    unsigned int index{};
    std::cin >> index;
    if(index == 0 || index > machines.size()){
        throw InvalidInputException("You must choose an existing machine.");
    }
    delete machines[index - 1];
    machines.erase(machines.begin() + index - 1);
    Logger::info("Machine removed");
}

void Menu::run() {
    int choice{-1};
    while(choice != 0) {
        std::cout << "\n===== ARGUS =====\n";
        std::cout << "1. Add machine\n";
        std::cout << "2. Add metric to machine\n";
        std::cout << "3. Collect metrics\n";
        std::cout << "4. Display all machines\n";
        std::cout << "5. Display alerts\n";
        std::cout << "6. Remove machine\n";
        std::cout << "0. Exit\n";
        std::cout << "> ";
        std::cin >> choice;
    
        try{
            switch(choice) {
                case 1: addMachine(); break;
                case 2: addMetric(); break;
                case 3: collectMetrics(); break;
                case 4: displayMachines(); break;
                case 5: displayAlerts(); break;
                case 6: removeMachine(); break;
                case 0: std::cout << "Shutting down Argus.\n"; break;
                default:
                    throw InvalidInputException("Option must be between 0 and 6");
            }
        } catch (const ArgusException& e) {
            std::cout << "[ERROR] " << e.what() << "\n";
            Logger::error(e.what());
        }
    }
}