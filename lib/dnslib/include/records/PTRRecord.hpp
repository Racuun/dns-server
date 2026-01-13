/**
 * @file PTRRecord.hpp
 * @brief Defines the PTRRecord class for representing a DNS 'PTR' (Pointer) record.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "ResourceRecord.hpp"
#include "../utils/utils.hpp"

namespace dnslib {

    /**
     * @brief Represents a DNS 'PTR' record, primarily used for reverse DNS lookups.
     * 
     * PTR records map an IP address (in the form of a specially formatted domain name)
     * back to a canonical domain name.
     */
    class PTRRecord : public ResourceRecord {
    private:
        std::string ptr;
    public:
        /**
         * @brief Constructs a new PTRRecord object.
         * 
         * @param n The domain name representing the IP address (e.g., "1.0.168.192.in-addr.arpa").
         * @param ttl The time-to-live for this record in seconds.
         * @param ptr The canonical domain name associated with the IP.
         */
        PTRRecord(std::string n, uint32_t ttl, std::string ptr)
            : ResourceRecord(n, 12, ttl), ptr(ptr) {
                utils::validateDomainName(ptr);
            }

        /**
         * @brief Serializes the PTRRecord into a byte buffer in DNS wire format.
         * 
         * @param buff The buffer to which the serialized data will be appended.
         */
        void serialize(std::vector<uint8_t>& buff) const override;

        /**
         * @brief Returns a string representation of the PTRRecord.
         * 
         * @return A std::string describing the PTRRecord, typically "NAME TTL IN PTR DOMAIN".
         */
        std::string toString() const override;
    
        /**
         * @brief Gets the domain name pointed to by this record.
         * 
         * @return The target domain name.
         */
        std::string getPtr() const { return ptr; }
    
    };
}
