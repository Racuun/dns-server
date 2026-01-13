#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>

#include "dns.hpp"


#include <thread>
#include <functional>

#include "message/DNSMessage.hpp"
#include "utils/etsqueue.hpp"
#include "connector.hpp"
#include "resolver.cpp"
#include "utils/log.hpp"


int main(int, char**){
    // --- DNS START ---

    utils::Logger::get().config(TARGET_CONSOLE, utils::LogLevel::DEBUG);

    LOG_INFO("--- DNS Server Starting ---");

    utils::ETSQueue<dnslib::DNSMessageL> qIn;
    utils::ETSQueue<dnslib::DNSMessageL> qOut;

    std::thread logicThread(testThread, std::ref(qIn), std::ref(qOut));
    LOG_INFO("Logic thread started");

    std::thread netThread(networkThread, 53, std::ref(qIn), std::ref(qOut));
    LOG_INFO("Network thread started");

    logicThread.join();
    netThread.join();

    return 0;
}