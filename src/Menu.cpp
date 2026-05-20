#include "../include/Menu.h"
#include "../include/Logger.h"
#include "../include/CPUMetric.h"
#include "../include/MemoryMetric.h"
#include "../include/DiskMetric.h"
#include "../include/NetworkMetric.h"
#include "../include/Exceptions.h"
#include "../include/MetricFactory.h"
#include "../include/ConfigManager.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <cassert>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iomanip>
#include <sstream>

using namespace ftxui;

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
    size_t index{};
    std::cin >> index;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InvalidInputException("Invalid input for machine index");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (index == 0 || index > machines.size())
        throw InvalidInputException("Machine index out of range");
    Machine* machine = machines[index - 1];

    std::cout << "\nMetric type (CPU/Memory/Disk/Network): ";
    std::string type;
    std::cin >> type;

    std::string name;
    std::cout << "\nName: ";
    std::cin >> name;

    double param1 = 0.0, param2 = 0.0;
    
    for(auto& c : type)
        c = tolower(c);
    
    auto readDouble = [](double& out) {
        std::cin >> out;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InvalidInputException("Invalid numeric input");
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    };

    if (type == "cpu") {
        std::cout << "Threshold (%): ";
        readDouble(param1);
    } else if (type == "memory" || type == "disk"){
        std::cout  << "Maximum capacity: ";
        readDouble(param1);
        std::cout << "Threshold (%): ";
        readDouble(param2);
    }


    Metric* m = MetricFactory::create(type, name, param1, param2);
    m->addObserver(&alertObserver);
    m->setMachineName(machine->getHostname());
    machine->addMetric(m);
    Logger::info("Metric added: " + name + " to " + machine->getHostname());
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
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InvalidInputException("Invalid input for machine index");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if(index == 0 || index > machines.size()){
        throw InvalidInputException("You must choose an existing machine.");
    }
    delete machines[index - 1];
    machines.erase(machines.begin() + index - 1);
    Logger::info("Machine removed");
}

void Menu::registerObservers(Machine* machine) {
    for (auto metric : machine->getMetrics()) {
        metric -> addObserver(&alertObserver);
        metric -> setMachineName(machine->getHostname());
    }
}

void Menu::showAddMachineScreen(ftxui::ScreenInteractive& screen) {
    std::string valueHostname = Machine::detectHostname();
    std::string valueIpAddress = Machine::detectIpAddress();

    auto isValidHostname = [](const std::string& s) -> bool {
        return !s.empty();
    };

    auto isValidIp = [](const std::string& s) -> bool {
        std::stringstream ss(s);
        std::string token;
        size_t cnt{};
        while(std::getline(ss, token, '.')) {
            cnt++;
            if(token.empty() || !std::all_of(token.begin(), token.end(), ::isdigit))
                return false;
            try {
                int val = std::stoi(token);
                if (val < 0 || val > 255) return false;
            } catch (...) { return false; }
        }
        if(cnt != 4) return false;
        return true;
    };

    bool hostnameError = false;
    bool ipError = false;

    auto inputHostname = Input(&valueHostname, "");

    auto inputIpAddress = Input(&valueIpAddress, "");

    int selectedField = 0;
    auto container = Container::Vertical( { inputHostname, inputIpAddress }, &selectedField);
    bool hostnameTyping = false;
    bool ipTyping = false;

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Hostname:"),
            inputHostname->Render() | (hostnameError ? color(Color::Red) : color(Color::White)),
            hostnameError ? text("Invalid Hostname") | color(Color::Red) : text(""),
            text("IP Address:"),
            inputIpAddress->Render() | (ipError ? color(Color::Red) : color(Color::White)),
            ipError ? text("Invalid IP Address") | color(Color::Red) : text(""),
        });
    });

    auto handler = CatchEvent(renderer, [&](Event event){
        if (event.is_character()) {
            if(selectedField == 0 && !hostnameTyping) {
                hostnameTyping = true;
                valueHostname = "";
            } else if(selectedField == 1 && !ipTyping){
                ipTyping = true;
                valueIpAddress = "";
            }
        }
        if (event == Event::Return) {
            hostnameError = !isValidHostname(valueHostname);
            ipError = !isValidIp(valueIpAddress);
            if (!hostnameError && !ipError) {
                Machine* machine = new Machine(valueHostname, valueIpAddress);
                machines.push_back(machine);
                screen.ExitLoopClosure()();
            }
            return true;
        }
        if (event == Event::Escape || event == Event::Character('q')) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });
    screen.Loop(handler);
}

void Menu::showAddMetricScreen(Machine* machine, ftxui::ScreenInteractive& screen) {
    int selectedField = 0;
    int selectedType = 0;
    std::vector<std::string>metricTypes = {"CPU Usage", "Memory Usage", "Disk Usage", "Network Usage"};
    auto typeList = ftxui::Menu(&metricTypes, &selectedType);

    std::string valueName = "";
    std::string valueMaxValue = "";
    std::string valueThreshold = "";
    bool nameError = false;
    bool maxValueError = false;
    bool thresholdError = false;

    auto isValidName = [](const std::string& s) -> bool {
        return !s.empty();
    };

    auto isValidDouble = [](const std::string& s) -> bool {
        if (s.empty()) return false;
        try {
            double val = std::stod(s);
            return val > 0;
        } catch (...) {
            return false;
        }
    };

    auto inputValueName = Input(&valueName, "");
    auto inputValueMaxValue = Input(&valueMaxValue, "");
    auto inputValueThreshold = Input(&valueThreshold, "");

    auto container = Container::Vertical({ typeList, inputValueName, inputValueMaxValue, inputValueThreshold }, &selectedField);

    auto renderer = Renderer(container, [&] {
        Elements inputElements;
        switch(selectedType) {
            case 0: // CPU
                inputElements.push_back(text("Name:"));
                inputElements.push_back(inputValueName->Render() | (nameError ? color(Color::Red) : color(Color::White)));
                inputElements.push_back(nameError ? text("Name invalid!") | color(Color::Red) : text(""));
                inputElements.push_back(text("Threshold:"));
                inputElements.push_back(inputValueThreshold->Render() | (thresholdError ? color(Color::Red) : color(Color::White)));
                inputElements.push_back(thresholdError ? text("Threshold invalid!") | color(Color::Red) : text(""));
                break;
            case 1: // Memory
            case 2: // Disk
                inputElements.push_back(text("Name:"));
                inputElements.push_back(inputValueName->Render() | (nameError ? color(Color::Red) : color(Color::White)));
                inputElements.push_back(nameError ? text("Name invalid!") | color(Color::Red) : text(""));
                inputElements.push_back(text("Max Value:"));
                inputElements.push_back(inputValueMaxValue->Render() | (maxValueError ? color(Color::Red) : color(Color::White)));
                inputElements.push_back(maxValueError ? text("Max Value invalid!") | color(Color::Red) : text(""));
                inputElements.push_back(text("Threshold:"));
                inputElements.push_back(inputValueThreshold->Render() | (thresholdError ? color(Color::Red) : color(Color::White)));
                inputElements.push_back(thresholdError ? text("Threshold invalid!") | color(Color::Red) : text(""));
                break;
            case 3: // Network
                inputElements.push_back(text("Name:"));
                inputElements.push_back(inputValueName->Render() | (nameError ? color(Color::Red) : color(Color::White)));
                inputElements.push_back(nameError ? text("Name invalid!") | color(Color::Red) : text(""));
                break;
        }
        return hbox({
            // in stanga, lista de tipuri de metrici
            vbox({typeList->Render()}) | size(WIDTH, EQUAL, 30),
            separator(),
            // in dreapta, campurile de input, afisate conditionat in functie de selectedType
            vbox(inputElements)
        });
    });

    auto handler = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Return) {
            switch(selectedType) {
                case 0: // CPU
                    nameError = !isValidName(valueName);
                    thresholdError = !isValidDouble(valueThreshold);
                    if (!nameError && !thresholdError){
                        Metric* m = new CPUMetric(valueName, std::stod(valueThreshold));
                        m->addObserver(&alertObserver);
                        m->setMachineName(machine->getHostname());
                        machine->addMetric(m);
                        screen.ExitLoopClosure()();
                    }
                    break;
                case 1: // Memory
                    nameError = !isValidName(valueName);
                    maxValueError = !isValidDouble(valueMaxValue);
                    thresholdError = !isValidDouble(valueThreshold);
                    if (!nameError && !maxValueError && !thresholdError){
                        Metric* m = new MemoryMetric(valueName, std::stod(valueMaxValue), std::stod(valueThreshold));
                        m->addObserver(&alertObserver);
                        m->setMachineName(machine->getHostname());
                        machine->addMetric(m);
                        screen.ExitLoopClosure()();
                    }
                    break;
                case 2: // Disk
                    nameError = !isValidName(valueName);
                    maxValueError = !isValidDouble(valueMaxValue);
                    thresholdError = !isValidDouble(valueThreshold);
                    if (!nameError && !maxValueError && !thresholdError){
                        Metric* m = new DiskMetric(valueName, std::stod(valueMaxValue), std::stod(valueThreshold));
                        m->addObserver(&alertObserver);
                        m->setMachineName(machine->getHostname());
                        machine->addMetric(m);
                        screen.ExitLoopClosure()();
                    }
                    break;
                case 3: // Network
                    nameError = !isValidName(valueName);
                    if (!nameError){
                        Metric* m = new NetworkMetric(valueName);
                        m->addObserver(&alertObserver);
                        m->setMachineName(machine->getHostname());
                        machine->addMetric(m);
                        screen.ExitLoopClosure()();
                    }
                    break;
            }
            return true;
        }
        if (event == Event::Escape) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });
    screen.Loop(handler);
}

void Menu::run() {
    // Load Config
    try {
        ConfigManager::load(machines, alerts, "config.json");
        // dupa load inregistram observatorii pe toate masinile incarcate
        for (auto m : machines)
            registerObservers(m);
        Logger::info("Configuration loaded from config.json");
    } catch (...) {
        Logger::info("No existing configuration found, starting from scratch");
    }

    auto screen = ScreenInteractive::Fullscreen();
    int selectedMachine = 0;
    std::string statusMessage = "Argus says hi!";
    std::vector<std::string> machinesRegistered; // numele masinilor
    auto machineList = ftxui::Menu(&machinesRegistered, &selectedMachine);

    auto fmt = [](double v) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << v;
        return oss.str();
    };

    auto refreshMachinesRegistered = [&]() {
        machinesRegistered.clear();
        for (const auto& m : machines)
            machinesRegistered.push_back(m->getHostname() + " (" + m->getIpAddress() + ")");
    };
    refreshMachinesRegistered();

    auto renderer = Renderer(machineList, [&] {
        Elements metricElements;
        if (!machines.empty() && selectedMachine < (int)machines.size()) {
            for (const auto& metric : machines[selectedMachine]->getMetrics()) {
                metricElements.push_back(
                    text(metric->getName() + ": " + fmt(metric->getValue()) + " " + metric->getUnit())
                );
            }
        }
        return vbox({
            text("==== ARGUS ====") | bold | color(Color::Cyan) | center,
            separator(),
            hbox({
                // in stanga, lista de masini inregistrate
                vbox({machineList->Render()}) | size(WIDTH, EQUAL, 30),
                separator(),
                // in dreapta, metricile masinii selectate
                vbox(metricElements)
                }),
            text(statusMessage) | color(Color::Green),
            separator(),
            text("[C]ollect   [A]dd machine   Add [M]etric   [D]elete machine   [Q]uit") | color(Color::Yellow),
        });
    });

    auto handler = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('a')) {
            showAddMachineScreen(screen);
            refreshMachinesRegistered();
            return true;
        }
        if (event == Event::Character('m')) {
            if(!machines.empty()){
                showAddMetricScreen(machines[selectedMachine], screen);
                return true;
            } else {
                return false;
            }
        }
        // eliberez memoria cu delete
        // o scot din machines
        // refresh la machinesRegistered
        // ajustez selectedMachine
        if (event == Event::Character('d')) {
            if(!machines.empty()){
                delete machines[selectedMachine];
                machines.erase(machines.begin() + selectedMachine);
                selectedMachine = machines.empty() ? 0 : std::min(selectedMachine, (int)machines.size() - 1);
                refreshMachinesRegistered();
                return true;
            }
            return false;
        }
        if (event == Event::Character('c')) {
            if(!machines.empty()){
                machines[selectedMachine]->collectAll();
                statusMessage = "Metrics collected for: " + machines[selectedMachine]->getHostname();
                return true;
            }
            return false;
        }
        if (event == Event::Character('q')) {
            ConfigManager::save(machines, alerts, "config.json");
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(handler);
    Logger::close();
}