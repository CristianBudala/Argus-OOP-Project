#pragma once
#include "Machine.h"
#include "Alert.h"
#include <vector>

class Menu {
public:

private:
    static Menu* instance;
    std::vector<Machine*> machines;
    std::vector<Alert> alerts;
};