#pragma once
#include "Machine.h"
#include "Alert.h"
#include "AlertObserver.h"
#include <vector>
#include <ftxui/component/screen_interactive.hpp>

class Menu {
private:
    Menu() {}
    
    std::vector<Machine*> machines;
    std::vector<Alert> alerts;

    AlertObserver alertObserver{alerts}; // initializat cu referinta la alerts

    void addMachine();
    void addMetric();
    void collectMetrics();
    void displayMachines() const;
    void displayAlerts() const;
    void removeMachine();
    void registerObservers(Machine* machine);
    void showAddMachineScreen(ftxui::ScreenInteractive& screen);
    void showAddMetricScreen(Machine* machine, ftxui::ScreenInteractive& screen);

public:
    ~Menu();
    
    static Menu& getInstance() {
        static Menu instance; // creat o singura data, distrus la sfarsit
        return instance;
    }

    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    void run();
};