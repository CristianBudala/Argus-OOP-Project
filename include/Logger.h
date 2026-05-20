#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>

class Logger {
public:
    enum class Level { INFO, WARNING, ERROR };

private:
    static std::ofstream logFile;
    static std::string logFilename;
    static bool toConsole;
    static bool toFile;
    static std::size_t maxFileSize;

    static void log(Level level, const std::string& message);

public:
    // Config methods
    static void init(const std::string& filename = "argus.log",
                     bool console = true, bool file = true,
                     std::size_t maxBytes = 1 * 1024 * 1024);
    static void close();

    // Log level methods
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
};