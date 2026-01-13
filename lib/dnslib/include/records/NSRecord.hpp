/**
 * @file NSRecord.hpp
 * @brief Defines the NSRecord class for representing a DNS 'NS' (Name Server) record.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "ResourceRecord.hpp"

namespace dnslib {

    /**
     * @brief Represents a DNS 'NS' record, which delegates a DNS zone to an authoritative name server.
     */
    class NSRecord : public ResourceRecord {
    private:
        std::string ns;
    public:
        /**
         * @brief Constructs a new NSRecord object.
         * 
         * @param n The domain name for which the name server is authoritative.
         * @param ttl The time-to-live for this record in seconds.
         * @param ns The domain name of the authoritative name server.
         */
        NSRecord(std::string n, uint32_t ttl, std::string ns)
            : ResourceRecord(n, 2, ttl), ns(ns) {}

        /**
         * @brief Serializes the NSRecord into a byte buffer in DNS wire format.
         * 
         * @param buff The buffer to which the serialized data will be appended.
         */
        void serialize(std::vector<uint8_t>& buff) const override;

        /**
         * @brief Returns a string representation of the NSRecord.
         * 
         * @return A std::string describing the NSRecord, typically "NAME TTL IN NS NAMESERVER".
         */
        std::string toString() const override;
    
        /**
         * @brief Gets the domain name of the name server.
         * 
         * @return The name server's domain name.
         */
        std::string getNs() const { return ns; }
    
    };

}