#include "../include/Logger.h"
#include <iomanip>
#include <ctime>

std::ofstream Logger::logFile;
bool Logger::toConsole = true;
bool Logger::toFile = false;

