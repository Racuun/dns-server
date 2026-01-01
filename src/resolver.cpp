#include "dns.hpp"
#include "message/DNSMessage.hpp"
#include "utils/etsqueue.hpp"
#include <exception>
#include <iostream>

inline void resolverWorker(
    utils::ETSQueue<dnslib::DNSMessageL>& inputQueue, 
    utils::ETSQueue<dnslib::DNSMessageL>& outputQueue
) {

    while (true) {

        dnslib::DNSMessageL message = inputQueue.pop();

        try {

            // Resolver code here

            // What should be done"
            // 1. parse buffored 
            // 2. If packet is request - send questions
            // 3. If pakcet is response - save it to use for responses to hosts
            // 4. If address is known serialize and put to queue
            // Don't forget to check cache

            //outputQueue.push(std::move(output));

        } catch (const std::exception& e) {
            std::cerr << "Error" << e.what() << "\n";
        }
    }

}