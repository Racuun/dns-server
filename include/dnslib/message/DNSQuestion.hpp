#pragma once

#include "../interface/ISerializable.hpp"
#include <cstdint>

namespace dnslib {

    enum class QTYPE : uint16_t {
        A = 1,
        NS = 2,
        PTR = 12,
        HINFO = 13,
        MINFO = 14,
        MX = 15,
        TXT = 16
    };
    
    enum class QCLASS : uint16_t {
        IN = 1
    };
    
    class DNSQuestion : ISerializable {
    private:
        std::string name;
        uint16_t type;
        uint16_t qclass;
    public:
        DNSQuestion(std::string name, uint16_t type, uint16_t qclass)
            : name(name), type(type), qclass(qclass) {};
        DNSQuestion(std::string name, QTYPE type, QCLASS qclass)
            : name(name), 
            type(static_cast<uint16_t>(type)), 
            qclass(static_cast<uint16_t>(qclass)) {};
        
        void serialize(std::vector<uint8_t>& buff) const override;
        std::string toString() const override;

        QTYPE getType() const { return static_cast<QTYPE>(type); }
        QCLASS getQclass() const { return static_cast<QCLASS>(qclass); }
    };
    
}

namespace std {
    inline std::string to_string(const dnslib::QTYPE& qtype) {
        switch (qtype) {
            case dnslib::QTYPE::A:
                return "A";
                break;
            case dnslib::QTYPE::NS:
                return "NS";
                break;
            case dnslib::QTYPE::PTR:
                return "PTR";
                break;
            case dnslib::QTYPE::HINFO:
                return "HINFO";
                break;
            case dnslib::QTYPE::MINFO:
                return "MINFO";
                break;
            case dnslib::QTYPE::MX:
                return "MX";
                break;
            case dnslib::QTYPE::TXT:
                return "TXT";
                break;
            default:
                return "Unknown";
                break;
        }
    }
}