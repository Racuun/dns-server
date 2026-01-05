#pragma once

#include "ResourceRecord.hpp"

namespace dnslib {

    class PTRRecord : public ResourceRecord {
    private:
        std::string ptr;
    public:
        PTRRecord(std::string n, uint32_t ttl, std::string ptr)
            : ResourceRecord(n, 12, ttl), ptr(ptr) {}

        void serialize(std::vector<uint8_t>& buff) const override;

        std::string toString() const override;
    
        std::string getPtr() const { return ptr; }
    
    };
}