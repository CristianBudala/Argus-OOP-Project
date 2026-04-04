#include "../include/Argus.h"

int main(){

    Logger::init("argus.log");
    Logger::info("Argus is waking up");
    
    /*
    CPUMetric cpu("cpu_usage", 90.0);
    MemoryMetric ram("ram_usage", 16200.0);
    DiskMetric disk("disk_usage", 512.0);
    NetworkMetric net("net_speed");

    cpu.collect();
    ram.collect();
    disk.collect();
    net.collect();

    std::cout << cpu << std::endl;
    std::cout << ram << std::endl;
    std::cout << disk << std::endl;
    std::cout << net << std::endl;

    // Test diamant
    std::cout << "---------------------" << std::endl;
    std::cout << "CPU exceeded threshold? " << (cpu.isExceeded() ? "YES" : "NO") << "\n";
    std::cout << "CPU usage %: " << cpu.getUsagePercent() << "%\n";
    */

    Menu::getInstance().run();


    Logger::close();
    return 0;
}