/**
 * @file DNSHeader.hpp
 * @brief Defines the DNSHeader class, which represents the header section of a DNS message.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "../interface/ISerializable.hpp"
#include <gtest/gtest_prod.h>
#include <cstdint>
#include <vector>
#include <string>

namespace dnslib {

    class PacketBuilder;
    
    /**
     * @brief Specifies the kind of query in a DNS message.
     */
    enum class OPCODE : uint8_t {
        QUERY = 0,  ///< A standard query.
        IQUERY = 1, ///< An inverse query (obsolete).
        STATUS = 2  ///< A server status request.
    };

    /**
     * @brief Defines the response code, indicating the status of a DNS query.
     */
    enum class RCODE : uint8_t {
        NOERROR = 0,        ///< No error condition.
        FORMATERROR = 1,    ///< The name server was unable to interpret the query.
        SERVERFAILURE = 2,  ///< The name server was unable to process this query due to a problem with the name server.
        NAMEERROR = 3,      ///< The domain name referenced in the query does not exist.
        NOTIMPLEMENTED = 4, ///< The name server does not support the requested kind of query.
        REFUSED = 5         ///< The name server refuses to perform the specified operation for policy reasons.
    };

    /**
     * @brief Represents the 12-byte header of a DNS packet.
     * 
     * The header contains fields that specify what the message is for,
     * whether it's a query or a response, and various flags that control
     * how the message is handled. It also includes counts for the number
     * of records in the subsequent sections of the packet.
     */
    class DNSHeader : public ISerializable {
        FRIEND_TEST(DnsHeaderTest, FlagManipulationWorks);
        FRIEND_TEST(DnsHeaderTest, SetAndGetId);

        friend class PacketBuilder;
    private:
        uint16_t id = 0;      ///< A 16-bit identifier assigned by the program that generates any kind of query.
        uint16_t flags = 0;   ///< A 16-bit field of flags. @see setResponse, setOpcode, etc.
        uint16_t qdCount = 0; ///< The number of entries in the question section.
        uint16_t anCount = 0; ///< The number of resource records in the answer section.
        uint16_t nsCount = 0; ///< The number of name server resource records in the authority records section.
        uint16_t arCount = 0; ///< The number of resource records in the additional records section.

        // Masks for accessing specific bits within the 16-bit flags field.
        static constexpr uint16_t QR_MASK     = 1 << 15; ///< Query/Response flag bit.
        static constexpr uint16_t OPCODE_MASK = 0xF << 11; ///< Opcode field bits.
        static constexpr uint16_t AA_MASK     = 1 << 10; ///< Authoritative Answer flag bit.
        static constexpr uint16_t TC_MASK     = 1 << 9;  ///< TrunCation flag bit.
        static constexpr uint16_t RD_MASK     = 1 << 8;  ///< Recursion Desired flag bit.
        static constexpr uint16_t RA_MASK     = 1 << 7;  ///< Recursion Available flag bit.
        static constexpr uint16_t AD_MASK     = 1 << 5;  ///< Authentic Data flag bit (DNSSEC).
        static constexpr uint16_t CD_MASK     = 1 << 4;  ///< Checking Disabled flag bit (DNSSEC).
        static constexpr uint16_t RCODE_MASK  = 0x000F;  ///< Response code field bits.

        /** @name Private Setters
         *  These methods are used internally, primarily by the PacketBuilder, to configure the header flags.
         */
        ///@{
        void setId(uint16_t i) { id = i; }
        void setResponse(bool r) { flags = r ? flags | QR_MASK : flags & ~QR_MASK; }
        void setQuery(bool q) { flags = q ? flags & ~QR_MASK : flags | QR_MASK; }
        void setAuthoritative(bool a) { flags = a ? flags | AA_MASK : flags & ~AA_MASK; }
        void setTruncated(bool t) { flags = t ? flags | TC_MASK : flags & ~TC_MASK; }
        void setRecursionDesired(bool rd) { flags = rd ? flags | RD_MASK : flags & ~RD_MASK; }
        void setRecursionAvailable(bool ra) { flags = ra ? flags | RA_MASK : flags & ~RA_MASK; }
        void setAuthenticData(bool ad) { flags = ad ? flags | AD_MASK : flags & ~AD_MASK; }
        void setCheckingDisabled(bool cd) { flags = cd ? flags | CD_MASK : flags & ~CD_MASK; }
        void setOpcode(OPCODE op) {
            flags &= ~OPCODE_MASK;
            flags |= (static_cast<uint16_t>(op) << 11) & OPCODE_MASK;
        }
        void setRcode(RCODE rc) {
            flags &= ~RCODE_MASK;
            flags |= static_cast<uint16_t>(rc) & RCODE_MASK;
        }
        ///@}

    public:
        /**
         * @brief Default constructor for a DNSHeader. Initializes all fields to zero.
         */
        DNSHeader() = default;

        /**
         * @brief Serializes the DNSHeader into DNS wire format.
         * 
         * @param buff The buffer to which the 12 bytes of the header will be appended in network byte order.
         */
        void serialize(std::vector<uint8_t>& buff) const override;
        /**
         * @brief Generates a human-readable string representation of the DNSHeader.
         * 
         * @return std::string A multi-line string detailing the header's fields and flags.
         */
        std::string toString() const override;

        /** @name Public Getters
         *  Accessor methods for header fields and flags.
         */
        ///@{

        /**
         * @brief Gets the 16-bit transaction ID.
         * @return The transaction ID.
         */
        uint16_t getId() const { return id; }

        /**
         * @brief Checks if the message is a response.
         * @return True if the QR flag is set, indicating a response.
         */
        bool isResponse() const { return flags & QR_MASK; }
        /**
         * @brief Checks if the message is a query.
         * @return True if the QR flag is not set, indicating a query.
         */
        bool isQuery() const { return !isResponse(); }
        
        /**
         * @brief Checks if the responding server is authoritative for the domain.
         * @return True if the AA (Authoritative Answer) flag is set.
         */
        bool authAns() const { return flags & AA_MASK; }
        
        /**
         * @brief Checks if the message was truncated.
         * @return True if the TC (TrunCation) flag is set.
         */
        bool truncation() const { return flags & TC_MASK; }
        
        /**
         * @brief Checks if the client desires recursive query handling.
         * @return True if the RD (Recursion Desired) flag is set.
         */
        bool recursionDesired() const { return flags & RD_MASK; }
        
        /**
         * @brief Checks if the server supports recursive queries.
         * @return True if the RA (Recursion Available) flag is set.
         */
        bool recursionAvailable() const { return flags & RA_MASK; }

        /**
         * @brief (DNSSEC) Checks if all the data included in the answer and authority sections is authentic.
         * @return True if the AD (Authentic Data) flag is set.
         */
        bool authenticData() const { return flags & AD_MASK; }

        /**
         * @brief (DNSSEC) Checks if the client disables signature validation.
         * @return True if the CD (Checking Disabled) flag is set.
         */
        bool checkingDisabled() const { return flags & CD_MASK; }
        
        /**
         * @brief Gets the operation code.
         * @return The OPCODE enum value.
         */
        OPCODE opcode() const { return static_cast<OPCODE>(((flags & OPCODE_MASK) >> 11)); }
        
        /**
         * @brief Gets the response code.
         * @return The RCODE enum value.
         */
        RCODE rcode() const { return static_cast<RCODE>(flags & RCODE_MASK); }
        ///@}
    };

}