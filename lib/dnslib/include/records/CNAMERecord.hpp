#pragma once

#include "ResourceRecord.hpp"

namespace dnslib {

    class CNAMERecord : public ResourceRecord {
    private:
        std::string cname;
    public:
        CNAMERecord(std::string n, uint32_t ttl, std::string cname)
            : ResourceRecord(n, 5, ttl), cname(cname) {}

        void serialize(std::vector<uint8_t>& buff) const override;

        std::string toString() const override;
    
        std::string getCname() const { return cname; }
    };

}