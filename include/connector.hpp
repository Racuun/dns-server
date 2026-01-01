#pragma once

#include "dns.hpp"
#include "message/DNSMessage.hpp"
#include "utils/etsqueue.hpp"

void setNonBlocking(int sockfd);
void networkThread(int port, utils::ETSQueue<dnslib::DNSMessageL>& inputQueue, utils::ETSQueue<dnslib::DNSMessageL>& outputQueue);
void testThread(utils::ETSQueue<dnslib::DNSMessageL>& inputQueue, utils::ETSQueue<dnslib::DNSMessageL>& outputQueue);