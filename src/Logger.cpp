#include "../include/Logger.h"
#include <iomanip>
#include <ctime>
#include <sstream> // for std::ostringstream

std::ofstream Logger::logFile;
bool Logger::toConsole = true;
bool Logger::toFile = false;

void Logger::init(const std::string& filename, bool console, bool file) {
    toConsole = console;
    toFile = file;
    if (toFile) {
        logFile.open(filename, std::ios::app);
        if(!logFile.is_open()) {
            std::cerr << "Logger: could not open " << filename << std::endl;
            toFile = false;
        }
    }
}

void Logger::close() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(Level level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::string levelStr;
    switch (level) {
        case Level::INFO:   levelStr = "INFO";   break;
        case Level::WARNING:   levelStr = "WARNING";   break;
        case Level::ERROR:   levelStr = "ERROR";   break;
    }

    std::ostringstream line;
    line << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
         << "] [" << levelStr << "] " << message;
         
    if (toConsole) std::cout << line.str() << std::endl;
    if (toFile && logFile.is_open()) logFile << line.str() << std::endl;
}

void Logger::info(const std::string& message) { log(Level::INFO, message); }
void Logger::warning(const std::string& message) { log(Level::WARNING, message); }
void Logger::error(const std::string& message) { log(Level::ERROR, message); }
