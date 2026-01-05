#pragma once

#include "../interface/ISerializable.hpp"
#include "../utils/utils.hpp"
#include "../utils/types.hpp"
#include <string>

namespace dnslib {

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

        std::string getName() const { return name; }
        uint16_t getType() const { return type; }
        uint32_t getTtl() const { return ttl; }
        /**
         * @brief Serializes the resource record into a byte buffer.
         * 
         * @param buff The buffer to which the serialized data will be written.
         */
        virtual void serialize(std::vector<uint8_t>& buff) const override {
            // Name 
            utils::writeDomain(buff, name);
            // Type
            utils::writeU16(buff, type);
            // Class
            utils::writeU16(buff, 1); // IN
            // TTL
            utils::writeU32(buff, ttl);
        }
    };

}