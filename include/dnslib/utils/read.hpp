#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <string>

namespace dnslib::utils {
    
    /**
     * @brief A utility class for reading bytes from a buffer.
     */
    class ByteReader {
    private:
        const std::vector<uint8_t>& buffer;
        size_t index;
    
    public:
        /**
         * @brief Construct a new Byte Reader object.
         * 
         * @param buff The buffer to read from.
         */
        ByteReader(const std::vector<uint8_t>& buff): buffer(buff), index(0) {}

        /**
         * @brief Checks if reading `n` bytes would go out of bounds.
         * 
         * @param n The number of bytes to read.
         * @throw std::out_of_range if reading `n` bytes would exceed the buffer's size.
         */
        void chceckBounds(size_t n) const;

        /**
         * @brief Reads a uint8_t value from the buffer.
         * 
         * @return The uint8_t value.
         */
        uint8_t readU8();

        /**
         * @brief Reads a uint16_t value from the buffer in network byte order.
         * 
         * @return The uint16_t value.
         */
        uint16_t readU16();

        /**
         * @brief Reads a uint32_t value from the buffer in network byte order.
         * 
         * @return The uint32_t value.
         */
        uint32_t readU32();

        /**
         * @brief Decodes and reads a domain name from the buffer.
         * 
         * @return The decoded domain name.
         */
        std::string readDomain();

        /**
         * @brief Gets the current reading position in the buffer.
         * 
         * @return The current position (index).
         */
        size_t position() const { return index; }

        /**
         * @brief Sets the reading position in the buffer.
         * 
         * @param pos The new position (index).
         */
        void setPosition(size_t pos) { index = pos; }

        /**
         * @brief Gets the underlying buffer.
         * 
         * @return A const reference to the buffer.
         */
        const std::vector<uint8_t>& getBuffer() const { return buffer; }

    };
}