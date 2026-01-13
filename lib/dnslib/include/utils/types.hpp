/**
 * @file types.hpp
 * @brief Defines common enums and types used throughout the dnslib library.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstdint>
#include <string>


namespace dnslib {

    /**
     * @brief Defines the type of a DNS query.
     */
    enum class TYPE : uint16_t {
        A = 1,      ///< Query for an IPv4 address.
        NS = 2,     ///< Query for a name server.
        CNAME = 5,  ///< Query for a canonical name.
        PTR = 12,   ///< Query for a pointer record.
        HINFO = 13, ///< Query for host information.
        MINFO = 14, ///< Query for mailbox information.
        MX = 15,    ///< Query for a mail exchange record.
        TXT = 16    ///< Query for a text record.
    };

    /**
     * @brief Defines the class of a DNS query, almost always 'IN' for internet.
     */
    enum class CLASS : uint16_t {
        IN = 1      ///< The Internet class.
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
    inline std::string to_string(const dnslib::TYPE& qtype) {
        switch (qtype) {
            case dnslib::TYPE::A:
                return "A";
            case dnslib::TYPE::NS:
                return "NS";
            case dnslib::TYPE::PTR:
                return "PTR";
            case dnslib::TYPE::HINFO:
                return "HINFO";
            case dnslib::TYPE::MINFO:
                return "MINFO";
            case dnslib::TYPE::MX:
                return "MX";
            case dnslib::TYPE::TXT:
                return "TXT";
            default:
                return "Unknown";
        }
    }
}
