#pragma once

#include "ResourceRecord.hpp"
#include <vector>
#include <cstdint>

namespace dnslib {

    /**
     * @brief Represents a generic or unknown DNS resource record.
     * 
     * This class is used when the specific record type is not implemented or unknown.
     * It stores the raw RDATA as a byte buffer.
     */
    class UnknownRecord : public ResourceRecord {
    private:
        std::vector<uint8_t> rdata;
    public:
        /**
         * @brief Constructs a new UnknownRecord object.
         * 
         * @param n The domain name.
         * @param t The resource record type.
         * @param ttl The time-to-live value.
         * @param data The raw RDATA bytes.
         */
        UnknownRecord(std::string n, uint16_t t, uint32_t ttl, const std::vector<uint8_t>& data) 
            : ResourceRecord(n, t, ttl), rdata(data) {}

        /**
         * @brief Serializes the UnknownRecord into a byte buffer.
         */
        void serialize(std::vector<uint8_t>& buff) const override;

        /**
         * @brief Returns a string representation of the UnknownRecord.
         */
        std::string toString() const override;

        /**
         * @brief Gets the raw RDATA.
         */
        const std::vector<uint8_t>& getRdata() const { return rdata; }
    };
}
