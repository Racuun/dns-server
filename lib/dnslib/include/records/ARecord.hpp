/**
 * @file ARecord.hpp
 * @brief Defines the ARecord class for representing a DNS 'A' (Address) record.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "ResourceRecord.hpp"
#include <arpa/inet.h>

namespace dnslib {

    /**
     * @brief Represents a DNS 'A' record, which maps a domain name to an IPv4 address.
     * 
     * This class inherits from ResourceRecord and encapsulates the data
     * specific to an A record, namely the 32-bit IPv4 address.
     */
    class ARecord : public ResourceRecord {
    private:
        std::uint32_t ipAdress;
    public:
        /**
         * @brief Constructs a new ARecord object from a 32-bit integer IPv4 address.
         * 
         * @param n The domain name associated with the record.
         * @param ttl The time-to-live for this record in seconds.
         * @param ip The IPv4 address as a 32-bit unsigned integer in host byte order.
         */
        ARecord(std::string n, uint32_t ttl, std::uint32_t ip) 
            : ResourceRecord(n, 1, ttl), ipAdress(ip) {}
        /**
         * @brief Constructs a new ARecord object from a string representation of an IPv4 address.
         * 
         * @param n The domain name associated with the record.
         * @param ttl The time-to-live for this record in seconds.
         * @param ip The IPv4 address as a standard dot-decimal notation string (e.g., "192.168.1.1").
         */
        ARecord(std::string n, uint32_t ttl, std::string ip) 
            : ResourceRecord(n, 1, ttl), ipAdress(inet_addr(ip.c_str())) {}

        /**
         * @brief Serializes the ARecord into a byte buffer in DNS wire format.
         * 
         * This method serializes the common ResourceRecord header and then appends
         * the 4-byte IPv4 address in network byte order.
         * 
         * @param buff The buffer to which the serialized data will be appended.
         */
        void serialize(std::vector<uint8_t>& buff) const override;

        /**
         * @brief Returns a string representation of the ARecord.
         * 
         * This is primarily for debugging and logging. The format is typically:
         * "NAME TTL IN A IP_ADDRESS".
         * 
         * @return A std::string describing the ARecord.
         */
        std::string toString() const override;

        /**
         * @brief Gets the IPv4 address stored in this record.
         * 
         * @return The 32-bit IPv4 address as an unsigned integer in network byte order.
         */
        std::uint32_t getIpAddress() const { return ipAdress; }
    };

}