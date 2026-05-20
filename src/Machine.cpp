#include "../include/Machine.h"
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

std::string Machine::detectHostname() {
    char buffer[256];
    gethostname(buffer, sizeof(buffer));
    std::string hostname(buffer);
    return hostname;
}

std::string Machine::detectIpAddress() {
    struct ifaddrs* ifaddr;
    getifaddrs(&ifaddr); // construieste o lista de interfete

    for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        // fiecare 'ifa' e o interfata de retea (eth0, wlan0, lo etc.)
        // Conditii: sa fie IPv4, sa nu fie loopback
        if (std::string(ifa->ifa_name) != "lo" && (ifa->ifa_addr->sa_family) == AF_INET) {
            char ip[INET_ADDRSTRLEN];
            struct sockaddr_in* addr = (struct sockaddr_in*)ifa->ifa_addr;
            inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);
            freeifaddrs(ifaddr);
            return std::string(ip);
        }
    }
    freeifaddrs(ifaddr);
    return "IP address unknown";
}

Machine::Machine(const std::string& hostname, const std::string& ipAddress)
    : hostname(hostname), ipAddress(ipAddress) {}

Machine::~Machine() {
    for (auto mt : metrics)
        delete mt;
}

void Machine::addMetric(Metric* m) { metrics.push_back(m); }

void Machine::collectAll() { 
    for (auto mt : metrics)
        mt->collect();
}

void Machine::displayAll() const {
    for (auto mt : metrics)
        std::cout << *mt << std::endl;
}

const std::string& Machine::getHostname() const { return hostname; }

const std::string& Machine::getIpAddress() const { return ipAddress; }

const std::vector<Metric*>& Machine::getMetrics() const { return metrics; }