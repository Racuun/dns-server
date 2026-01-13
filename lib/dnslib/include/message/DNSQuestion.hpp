/**
 * @file DNSQuestion.hpp
 * @brief Defines the DNSQuestion class and related enums (QTYPE, QCLASS) that form the question section of a DNS message.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "../interface/ISerializable.hpp"
#include "../utils/types.hpp"
#include "../utils/utils.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace dnslib {
    
    /**
     * @brief Represents a single question in the question section of a DNS message.
     * 
     * A DNS query message contains one or more questions. Each question consists of a
     * domain name (qname), a query type (qtype), and a query class (qclass).
     */
    class DNSQuestion : public ISerializable {
    private:
        std::string name;
        uint16_t type;
        uint16_t qclass;
    public:
        /**
         * @brief Constructs a new DNSQuestion object using raw integer types.
         * 
         * @param name The domain name being queried (e.g., "www.example.com").
         * @param type The query type as a 16-bit integer.
         * @param qclass The query class as a 16-bit integer.
         */
        DNSQuestion(std::string name, uint16_t type, uint16_t qclass)
            : name(name), type(type), qclass(qclass) {
                utils::validateDomainName(name);
            }

        /**
         * @brief Constructs a new DNSQuestion object using strongly-typed enums.
         * 
         * @param name The domain name being queried (e.g., "www.example.com").
         * @param type The query type as a QTYPE enum value.
         * @param qclass The query class as a QCLASS enum value.
         */
        DNSQuestion(std::string name, TYPE type, CLASS qclass)
            : name(name), 
            type(static_cast<uint16_t>(type)), 
            qclass(static_cast<uint16_t>(qclass)) {
                utils::validateDomainName(name);
            }
        
        /**
         * @brief Serializes the DNSQuestion into DNS wire format.
         * 
         * Encodes the domain name, qtype, and qclass into the provided buffer.
         * 
         * @param buff The buffer to which the serialized question will be appended.
         */
        void serialize(std::vector<uint8_t>& buff) const override;

        /**
         * @brief Generates a human-readable string representation of the DNSQuestion.
         * 
         * @return std::string A string in the format "NAME QTYPE QCLASS".
         */
        std::string toString() const override;

        /**
         * @brief Gets the query type.
         * 
         * @return QTYPE The query type as a QTYPE enum.
         */
        TYPE getType() const { return static_cast<TYPE>(type); }

        /**
         * @brief Gets the query class.
         * 
         * @return QCLASS The query class as a QCLASS enum.
         */
        CLASS getClass() const { return static_cast<CLASS>(qclass); }

        /**
         * @brief Gets the domain name being queried.
         * 
         * @return std::string The query domain name.
         */
        std::string getName() const { return name; }
    };
    
}