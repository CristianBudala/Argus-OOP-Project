#include "../include/Menu.h"
#include "../include/Logger.h"
#include <iostream>
#include <limits>
#include <string>

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
    }
    std::cout << "Metrics collected for all machines.\n";
}
/*
Afișezi lista de mașini și ceri utilizatorului să aleagă una (după index)
Afișezi tipurile de metrici disponibile (CPU, Memory, Disk, Network)
Citești parametrii specifici tipului ales
Adaugi metrica la mașina selectată
*/
void Menu::addMetric() {
    if (machines.empty()){
        std::cout << "No machines added yet.\n";
        return;
    }
    size_t cnt{};
    for (const auto& m: machines){
        std::cout << cnt++ << ". " << m->getHostname() 
                    << " (" << m->getIpAddress() << ")\n";
    }
    std::cout << "Choose machine: ";
    size_t index;
    std::cin >> index;
    if (index >= machines.size())
        throw InvalidInputException("Machine index out of range");
    Machine* machine = machines[index];

    
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
    }
}