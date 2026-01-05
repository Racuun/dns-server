#pragma once

#include "ResourceRecord.hpp"
#include <cstdint>
#include <string>

namespace dnslib {

    class MXRecord : public ResourceRecord {
    private:
        std::uint16_t preference;
        std::string exchange;
    public:
        MXRecord(std::string n, uint32_t ttl, std::uint16_t pref, std::string exch)
            : ResourceRecord(n, 15, ttl), preference(pref), exchange(exch) {}
        
        void serialize(std::vector<uint8_t>& buff) const override;

        std::string toString() const override;

        std::uint16_t getPreference() const { return preference; }
        
        std::string getExchange() const { return exchange; }
    };

}