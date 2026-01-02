/**
 * @file PacketBuilder.hpp
 * @brief Defines the PacketBuilder class for constructing DNS packets programmatically.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "../message/DNSPacket.hpp"
#include <cstddef>
#include <cstdint>

// aliases for improved developer experience
#define F_NONE            dnslib::PacketFlag::NONE
#define F_RESPONSE        dnslib::PacketFlag::RESPONSE
#define F_AUTHORITATIVE   dnslib::PacketFlag::AUTHORITATIVE
#define F_TRUNCATED       dnslib::PacketFlag::TRUNCATED
#define F_RECURSION_DES   dnslib::PacketFlag::RECURSION_DES
#define F_RECURSION_AVAIL dnslib::PacketFlag::RECURSION_AVAIL

namespace dnslib {

    /**
     * @brief Represents the flags in a DNS packet header.
     * 
     * These flags are used to indicate various properties of the DNS message,
     * such as whether it's a response, if the server is authoritative, etc.
     */
    enum class PacketFlag : uint16_t {
        NONE            = 0,         ///< No flags set.
        RESPONSE        = 1 << 15,   ///< Message is a response.
        AUTHORITATIVE   = 1 << 10,   ///< Server is authoritative for the domain.
        TRUNCATED       = 1 << 9,    ///< Message was truncated.
        RECURSION_DES   = 1 << 8,    ///< Recursion is desired.
        RECURSION_AVAIL = 1 << 7     ///< Recursion is available.
    };

    /**
     * @brief Performs a bitwise OR operation on two PacketFlag values.
     * 
     * @param a The first PacketFlag.
     * @param b The second PacketFlag.
     * @return The result of the bitwise OR operation.
     */
    inline PacketFlag operator|(PacketFlag a, PacketFlag b) {
        return static_cast<PacketFlag>(
            static_cast<uint16_t>(a) | static_cast<uint16_t>(b)
        );
    }

    /**
     * @brief Performs a bitwise AND operation to check if a flag is set.
     * 
     * @param a The PacketFlag value containing the set of flags.
     * @param b The specific flag to check for.
     * @return True if the flag `b` is set in `a`, false otherwise.
     */
    inline bool operator&(PacketFlag a, PacketFlag b) {
        return (static_cast<uint16_t>(a) & static_cast<uint16_t>(b)) != 0;
    }


    /**
     * @brief A fluent builder for creating DNSPacket objects.
     * 
     * This class provides a set of methods to construct a DNSPacket step-by-step,
     * allowing for a more readable and less error-prone way of creating DNS messages.
     */
    class PacketBuilder {
        DNSPacket packet;
    public:
        /**
         * @brief Default constructor for a new PacketBuilder.
         */
        PacketBuilder() = default;

        /**
         * @brief Constructs a new PacketBuilder with a specific transaction ID.
         * 
         * @param id The 16-bit transaction ID for the DNS packet.
         */
        PacketBuilder(uint16_t id);

        /**
         * @brief Sets the transaction ID of the DNS packet.
         * 
         * @param id The 16-bit transaction ID.
         * @return A reference to the PacketBuilder for method chaining.
         */
        PacketBuilder& setId(uint16_t id);

        /**
         * @brief Sets the operation code (opcode) for the DNS packet.
         * 
         * @param op The OPCODE to set (e.g., QUERY, IQUERY, STATUS).
         * @return A reference to the PacketBuilder for method chaining.
         */
        PacketBuilder& withOpcode(OPCODE op);

        /**
         * @brief Sets the response code (RCODE) for the DNS packet.
         * 
         * @param rc The RCODE to set (e.g., NO_ERROR, FORMAT_ERROR).
         * @return A reference to the PacketBuilder for method chaining.
         */
        PacketBuilder& withRcode(RCODE rc);

        /**
         * @brief Sets the flags for the DNS packet using the PacketFlag enum.
         * 
         * @param flags A combination of PacketFlag values.
         * @return A reference to the PacketBuilder for method chaining.
         */
        PacketBuilder& withFlags(PacketFlag flags);

        /**
         * @brief Sets the flags for the DNS packet using a raw 16-bit integer.
         * 
         * @param flags The raw 16-bit flags field.
         * @return A reference to the PacketBuilder for method chaining.
         */
        PacketBuilder& withRawFlags(uint16_t flags);

        /**
         * @brief Adds a question to the DNS packet.
         * 
         * @param name The domain name being queried.
         * @param type The type of query (e.g., A, AAAA, MX).
         * @return A reference to the PacketBuilder for method chaining.
         */
        PacketBuilder& addQuestion(std::string name, TYPE type);

        /**
         * @brief Adds an answer resource record to the DNS packet.
         * 
         * @param record A shared pointer to a ResourceRecord object.
         * @return A reference to the PacketBuilder for method chaining.
         */
        PacketBuilder& addAnswer(std::shared_ptr<ResourceRecord> record);

        /**
         * @brief Sets the expected number of answer records.
         * This is typically used for responses.
         * 
         * @param count The number of answers that will be added.
         * @return A reference to the PacketBuilder for method chaining.
         */
        PacketBuilder& expectedAnswers(size_t count);

        /**
         * @brief Finalizes the construction and returns the DNSPacket.
         * 
         * @return The constructed DNSPacket object.
         */
        DNSPacket build();
    };
}