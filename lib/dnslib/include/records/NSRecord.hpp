#pragma once

#include "ResourceRecord.hpp"

namespace dnslib {

    class NSRecord : public ResourceRecord {
    private:
        std::string ns;
    public:
        NSRecord(std::string n, uint32_t ttl, std::string ns)
            : ResourceRecord(n, 2, ttl), ns(ns) {}

        void serialize(std::vector<uint8_t>& buff) const override;

        std::string toString() const override;
    
        std::string getNs() const { return ns; }
    
    };

}