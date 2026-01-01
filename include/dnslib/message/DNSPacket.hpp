/**
 * @file DNSPacket.hpp
 * @brief Defines the DNSPacket class, which represents a complete DNS message.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "DNSHeader.hpp"
#include "DNSQuestion.hpp"
#include "../records/ResourceRecord.hpp"
#include <memory>

namespace dnslib {

    class PacketBuilder;

    /**
     * @brief Represents a complete DNS packet, including header, questions, and resource records.
     * 
     * This class aggregates all the components of a DNS message: the header,
     * a list of questions, and lists for answer, authority, and additional
     * resource records. It provides methods for serialization and inspection.
     */
    class DNSPacket : public ISerializable {
        friend class PacketBuilder;
    private:
        DNSHeader header;
        std::vector<DNSQuestion> questions;
        
        std::vector<std::shared_ptr<ResourceRecord>> answers;
    public:
        /**
         * @brief Default constructor for a new DNSPacket object.
         */
        DNSPacket() = default;

        /**
         * @brief Serializes the entire DNSPacket into a byte buffer for network transmission.
         * 
         * This method calls the serialize methods of the header, each question, and
         * each resource record in the respective sections, appending them to the buffer.
         * 
         * @param buff The buffer to which the serialized packet data will be appended.
         */
        void serialize(std::vector<uint8_t>& buff) const override;

        /**
         * @brief Generates a human-readable string representation of the DNSPacket.
         * 
         * Useful for logging and debugging, this method provides a multi-line string
         * detailing the contents of the header, questions, and all record sections.
         * 
         * @return A std::string containing the formatted packet information.
         */
        std::string toString() const override;

        /**
         * @brief Gets a constant reference to the DNSHeader of the packet.
         * 
         * @return const DNSHeader& A reference to the packet's header.
         */
        const DNSHeader& getHeader() const { return header; }

        /**
         * @brief Gets a constant reference to the vector of DNSQuestions.
         * 
         * @return const std::vector<DNSQuestion>& A reference to the list of questions.
         */
        const std::vector<DNSQuestion>& getQuestions() const { return questions; }

        /**
         * @brief Gets a constant reference to the vector of answer ResourceRecords.
         * 
         * @return const std::vector<std::shared_ptr<ResourceRecord>>& A reference to the list of answers.
         */
        const std::vector<std::shared_ptr<ResourceRecord>>& getAnswers() const { return answers; }
    };

}