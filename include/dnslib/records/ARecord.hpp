#pragma once

#include "ResourceRecord.hpp"
#include <arpa/inet.h>

namespace dnslib {

    class ARecord : public ResourceRecord {
    private:
        std::uint32_t ipAdress;
    public:
        ARecord(std::string n, uint32_t ttl, std::uint32_t ip) 
            : ResourceRecord(n, 1, ttl), ipAdress(ip) {}
        ARecord(std::string n, uint32_t ttl, std::string ip) 
            : ResourceRecord(n, 1, ttl), ipAdress(inet_addr(ip.c_str())) {}

        void serialize(std::vector<uint8_t>& buff) const override;
        std::string toString() const override;
    };

}