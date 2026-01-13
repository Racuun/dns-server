/**
 * @file utils.hpp
 * @brief Provides utility functions for low-level binary data manipulation (writing).
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace dnslib::utils {

    /**
     * @brief Writes a uint8_t value to a buffer.
     * 
     * @param buffer The buffer to write to.
     * @param value The uint8_t value to write.
     */
    void writeU8(std::vector<uint8_t>& buffer, uint8_t value);

    /**
     * @brief Writes a uint16_t value to a buffer in network byte order.
     * 
     * @param buffer The buffer to write to.
     * @param value The uint16_t value to write.
     */
    void writeU16(std::vector<uint8_t>& buffer, uint16_t value);

    /**
     * @brief Writes a uint32_t value to a buffer in network byte order.
     * 
     * @param buffer The buffer to write to.
     * @param value The uint32_t value to write.
     */
    void writeU32(std::vector<uint8_t>& buffer, uint32_t value);

    /**
     * @brief Encodes and writes a domain name to a buffer.
     * 
     * @param buffer The buffer to write to.
     * @param name The domain name to encode and write.
     */
    void writeDomain(std::vector<uint8_t>& buffer, std::string name);

    /**
     * @brief Validates a domain name.
     * 
     * Checks if the domain name adheres to DNS standards (RFC 1035):
     * - Total length must be <= 253 characters.
     * - Labels must be <= 63 characters.
     * 
     * @param name The domain name to validate.
     * @throw std::invalid_argument if the domain name is invalid.
     */
    void validateDomainName(const std::string& name);

}
