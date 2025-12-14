#pragma once

#include "../interface/ISerializable.hpp"
#include <string>

namespace dnslib {

    enum class RTYPE : uint16_t {
        A = 1,
        NS = 2,
        PTR = 12,
        HINFO = 13,
        MINFO = 14,
        MX = 15,
        TXT = 16
    };
    

    class ResourceRecord : public ISerializable {
    protected:
        std::string name;
        uint16_t type;
        uint32_t ttl;
    public:
        ResourceRecord(std::string n, uint16_t t, uint32_t ttl) 
            : name(n), type(t), ttl(ttl) {}

        virtual ~ResourceRecord() = default;
        virtual void serialize(std::vector<uint8_t>& buff) const override = 0;
    };

}