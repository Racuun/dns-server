#pragma once

#include "../interface/ISerializable.hpp"
#include <string>

namespace dnslib {

    /**
     * @brief Enum for DNS Resource Record (RR) types.
     */
    enum class RTYPE : uint16_t {
        A = 1,      /**< Host Address */
        NS = 2,     /**< Authoritative Name Server */
        PTR = 12,   /**< Domain Name Pointer */
        HINFO = 13, /**< Host Information */
        MINFO = 14, /**< Mailbox or Mail List Information */
        MX = 15,    /**< Mail Exchange */
        TXT = 16    /**< Text strings */
    };
    

    /**
     * @brief Abstract base class for all DNS Resource Records (RRs).
     * 
     * This class provides a common interface for different types of resource records.
     * It defines the basic properties of an RR, such as name, type, and TTL.
     */
    class ResourceRecord : public ISerializable {
    protected:
        std::string name; /**< The domain name to which this resource record pertains. */
        uint16_t type;    /**< The type of the resource record (e.g., A, NS, MX). */
        uint32_t ttl;     /**< The time-to-live of the resource record in seconds. */

    public:
        /**
         * @brief Construct a new Resource Record object.
         * 
         * @param n The domain name.
         * @param t The resource record type.
         * @param ttl The time-to-live value.
         */
        ResourceRecord(std::string n, uint16_t t, uint32_t ttl) 
            : name(n), type(t), ttl(ttl) {}

        /**
         * @brief Destroy the Resource Record object.
         */
        virtual ~ResourceRecord() = default;

        /**
         * @brief Serializes the resource record into a byte buffer.
         * 
         * @param buff The buffer to which the serialized data will be written.
         */
        virtual void serialize(std::vector<uint8_t>& buff) const override = 0;
    };

}