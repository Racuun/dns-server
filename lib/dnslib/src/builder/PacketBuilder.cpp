#include "builder/PacketBuilder.hpp"

namespace dnslib {
    PacketBuilder::PacketBuilder(uint16_t id) {
        packet.header.id = id;
    }


    PacketBuilder& PacketBuilder::setId(uint16_t id) {
        packet.header.id = id;
        return *this;
    }

    PacketBuilder& PacketBuilder::withOpcode(OPCODE op) {
        packet.header.setOpcode(op);
        return *this;
    }

    PacketBuilder& PacketBuilder::withRcode(RCODE rc) {
        packet.header.setRcode(rc);
        return *this;
    }

    PacketBuilder& PacketBuilder::withFlags(PacketFlag flags) {
        packet.header.setResponse(flags & PacketFlag::RESPONSE);
        packet.header.setAuthoritative(flags & PacketFlag::AUTHORITATIVE);
        packet.header.setTruncated(flags & PacketFlag::TRUNCATED);
        packet.header.setRecursionDesired(flags & PacketFlag::RECURSION_DES);
        packet.header.setRecursionAvailable(flags & PacketFlag::RECURSION_AVAIL);
        return *this;
    }

    PacketBuilder& PacketBuilder::withRawFlags(uint16_t flags) {
        packet.header.flags = flags;
        return *this;
    }



    PacketBuilder& PacketBuilder::addQuestion(std::string name, TYPE type) {
        packet.questions.push_back(DNSQuestion(name, type, CLASS::IN));
        packet.header.qdCount++;
        return *this;
    }

    PacketBuilder& PacketBuilder::addAnswer(std::shared_ptr<ResourceRecord> record) {
        packet.answers.push_back(record);
        packet.header.anCount++;
        return *this;
    }

    PacketBuilder& PacketBuilder::addAuthority(std::shared_ptr<ResourceRecord> record) {
        packet.authority.push_back(record);
        packet.header.auCount++;
        return *this;
    }

    PacketBuilder& PacketBuilder::addAdditional(std::shared_ptr<ResourceRecord> record) {
        packet.additional.push_back(record);
        packet.header.arCount++;
        return *this;
    }


    PacketBuilder& PacketBuilder::expectedAnswers(size_t count) {
        packet.answers.reserve(count);
        return *this;
    }


    DNSPacket PacketBuilder::build() {
        return std::move(packet);
    }
}