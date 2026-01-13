/**
 * @file CNAMERecord.hpp
 * @brief Defines the CNAMERecord class for representing a DNS 'CNAME' (Canonical Name) record.
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
     * @brief Represents a DNS 'CNAME' record, which aliases one name to another.
     * 
     * The CNAME record causes the redirection of a single domain name to another
     * domain name, which is referred to as the "canonical name".
     */
    class CNAMERecord : public ResourceRecord {
    private:
        std::string cname;
    public:
        /**
         * @brief Constructs a new CNAMERecord object.
         * 
         * @param n The alias domain name.
         * @param ttl The time-to-live for this record in seconds.
         * @param cname The canonical (target) domain name.
         */
        CNAMERecord(std::string n, uint32_t ttl, std::string cname)
            : ResourceRecord(n, 5, ttl), cname(cname) {
                utils::validateDomainName(cname);
            }

        /**
         * @brief Serializes the CNAMERecord into a byte buffer in DNS wire format.
         * 
         * @param buff The buffer to which the serialized data will be appended.
         */
        void serialize(std::vector<uint8_t>& buff) const override;

        /**
         * @brief Returns a string representation of the CNAMERecord.
         * 
         * @return A std::string describing the CNAMERecord, typically "NAME TTL IN CNAME TARGET".
         */
        std::string toString() const override;
    
        /**
         * @brief Gets the canonical name stored in this record.
         * 
         * @return The target domain name.
         */
        std::string getCname() const { return cname; }
    };

}
