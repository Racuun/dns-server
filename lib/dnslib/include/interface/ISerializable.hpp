/**
 * @file ISerializable.hpp
 * @brief Defines the ISerializable interface for objects that can be converted to a byte stream.
 * @version 1.0
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace dnslib {

    /**
     * @brief Interface for objects that can be serialized into a byte buffer.
     * 
     * This pure virtual class defines a standard interface for serialization. Any class
     * that represents a part of a DNS message and needs to be converted into a
     * byte stream for network transmission or file storage should implement this interface.
     */
    class ISerializable {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~ISerializable() = default;

        /**
         * @brief Serializes the object into a byte buffer.
         * 
         * This pure virtual function must be implemented by derived classes. It should
         * append the byte representation of the object to the provided buffer.
         * 
         * @param buff The vector of bytes to which the object's data will be appended.
         */
        virtual void serialize(std::vector<uint8_t>& buff) const = 0;

        /**
         * @brief Returns a string representation of the object.
         * 
         * This pure virtual function is intended for debugging and logging purposes.
         * It should provide a human-readable summary of the object's contents.
         * 
         * @return A std::string describing the object.
         */
        virtual std::string toString() const = 0;
    };

}