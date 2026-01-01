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
#include <cstdint>
#include <string>
#include <vector>

namespace dnslib {

    /**
     * @brief Defines the type of a DNS query.
     */
    enum class QTYPE : uint16_t {
        A = 1,      ///< Query for an IPv4 address.
        NS = 2,     ///< Query for a name server.
        PTR = 12,   ///< Query for a pointer record.
        HINFO = 13, ///< Query for host information.
        MINFO = 14, ///< Query for mailbox information.
        MX = 15,    ///< Query for a mail exchange record.
        TXT = 16    ///< Query for a text record.
    };
    
    /**
     * @brief Defines the class of a DNS query, almost always 'IN' for internet.
     */
    enum class QCLASS : uint16_t {
        IN = 1      ///< The Internet class.
    };
    
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
            : name(name), type(type), qclass(qclass) {};

        /**
         * @brief Constructs a new DNSQuestion object using strongly-typed enums.
         * 
         * @param name The domain name being queried (e.g., "www.example.com").
         * @param type The query type as a QTYPE enum value.
         * @param qclass The query class as a QCLASS enum value.
         */
        DNSQuestion(std::string name, QTYPE type, QCLASS qclass)
            : name(name), 
            type(static_cast<uint16_t>(type)), 
            qclass(static_cast<uint16_t>(qclass)) {};
        
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
        QTYPE getType() const { return static_cast<QTYPE>(type); }

        /**
         * @brief Gets the query class.
         * 
         * @return QCLASS The query class as a QCLASS enum.
         */
        QCLASS getQclass() const { return static_cast<QCLASS>(qclass); }

        /**
         * @brief Gets the domain name being queried.
         * 
         * @return std::string The query domain name.
         */
        std::string getName() const { return name; }
    };
    
}

namespace std {
    /**
     * @brief Overloads std::to_string for dnslib::QTYPE.
     * 
     * Provides a convenient way to get a string representation of a QTYPE enum value.
     * 
     * @param qtype The QTYPE value to convert.
     * @return std::string The string name of the query type (e.g., "A", "NS"). Returns "Unknown" if the type is not recognized.
     */
    inline std::string to_string(const dnslib::QTYPE& qtype) {
        switch (qtype) {
            case dnslib::QTYPE::A:
                return "A";
            case dnslib::QTYPE::NS:
                return "NS";
            case dnslib::QTYPE::PTR:
                return "PTR";
            case dnslib::QTYPE::HINFO:
                return "HINFO";
            case dnslib::QTYPE::MINFO:
                return "MINFO";
            case dnslib::QTYPE::MX:
                return "MX";
            case dnslib::QTYPE::TXT:
                return "TXT";
            default:
                return "Unknown";
        }
    }
}