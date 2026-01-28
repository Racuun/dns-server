#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <unistd.h>
#include <string.h>

#include "dns.hpp"


#include <thread>
#include <functional>

#include "message/DNSMessage.hpp"
#include "utils/etsqueue.hpp"
#include "connector.hpp"
#include "resolver.cpp"
#include "utils/log.hpp"
#include "cache.hpp"


int main(int argc, char** argv){
    
    bool runAsDaemon = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--daemon") == 0) {
            runAsDaemon = true;
        }
    }
    
    if (runAsDaemon) {
        if (daemon(0, 0) == -1) {
            std::cerr << "Failed to daemonize" << std::endl;
            return 1;
        }
        utils::Logger::get().config(TARGET_SYSLOG("dns-server"), utils::LogLevel::INFO);
    } else {
        utils::Logger::get().config(TARGET_CONSOLE, utils::LogLevel::DEBUG);
    }
    
    // --- DNS START ---
    DNS_LOG_INFO("--- DNS Server Starting ---");

    TLRUCache dnsCache(1000);

    utils::ETSQueue<dnslib::DNSMessageL> qIn;
    utils::ETSQueue<dnslib::DNSMessageL> qOut;

    /*
    std::thread logicThread(testThread, std::ref(qIn), std::ref(qOut));
    DNS_LOG_INFO("Logic thread started");
    */

    std::thread logicThread(resolverWorker, std::ref(qIn), std::ref(qOut), std::ref(dnsCache));
    DNS_LOG_INFO("Logic thread (Resolver) started");

    std::thread netThread(networkThread, 53, std::ref(qIn), std::ref(qOut));
    DNS_LOG_INFO("Network thread started");

    logicThread.join();
    netThread.join();

    return 0;
}
