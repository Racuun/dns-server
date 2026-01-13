/**
 * @file MXRecord.hpp
 * @brief Defines the MXRecord class for representing a DNS 'MX' (Mail Exchange) record.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "ResourceRecord.hpp"
#include <cstdint>
#include <string>

namespace dnslib {

    /**
     * @brief Represents a DNS 'MX' record, used to route email.
     * 
     * MX records specify the mail server responsible for accepting email messages
     * on behalf of a recipient's domain, along with a preference value used to
     * prioritize mail delivery if multiple servers are available.
     */
    class MXRecord : public ResourceRecord {
    private:
        std::uint16_t preference;
        std::string exchange;
    public:
        /**
         * @brief Constructs a new MXRecord object.
         * 
         * @param n The domain name associated with the record.
         * @param ttl The time-to-live for this record in seconds.
         * @param pref The preference value (lower is higher priority).
         * @param exch The domain name of the mail server.
         */
        MXRecord(std::string n, uint32_t ttl, std::uint16_t pref, std::string exch)
            : ResourceRecord(n, 15, ttl), preference(pref), exchange(exch) {}
        
        /**
         * @brief Serializes the MXRecord into a byte buffer in DNS wire format.
         * 
         * @param buff The buffer to which the serialized data will be appended.
         */
        void serialize(std::vector<uint8_t>& buff) const override;

        /**
         * @brief Returns a string representation of the MXRecord.
         * 
         * @return A std::string describing the MXRecord, typically "NAME TTL IN MX PREF EXCHANGE".
         */
        std::string toString() const override;

        /**
         * @brief Gets the preference value of the mail server.
         * 
         * @return The 16-bit preference integer.
         */
        std::uint16_t getPreference() const { return preference; }
        
        /**
         * @brief Gets the domain name of the mail server.
         * 
         * @return The exchange domain name.
         */
        std::string getExchange() const { return exchange; }
    };

}