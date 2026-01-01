/**
 * @file PacketParser.hpp
 * @brief Defines the PacketParser class for converting a raw byte buffer into a DNSPacket object.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstdint>
#include <vector>


namespace dnslib {

    class DNSPacket;

    class PacketBuilder;

    /**
     * @brief A parser for converting raw byte data into a structured DNSPacket.
     * 
     * This class provides a static method to take a buffer of bytes received from a
     * network socket or read from a file and transform it into a high-level DNSPacket
     * object, which can then be easily inspected and manipulated.
     */
    class PacketParser{
    public:
        /**
         * @brief Parses a raw byte vector into a DNSPacket object.
         * 
         * This function reads the DNS header, questions, answers, and other sections
         * from the provided data buffer. It uses a ByteReader and RecordFactory internally
         * to handle the low-level parsing details and construct the full object.
         * 
         * @param rawData A constant reference to a vector of uint8_t containing the raw DNS packet data.
         * @return A DNSPacket object representing the structured content of the raw data.
         * @throw std::out_of_range if the packet is malformed or truncated, leading to a read past the end of the buffer.
         */
        static DNSPacket parse(const std::vector<uint8_t>& rawData);
    };

}