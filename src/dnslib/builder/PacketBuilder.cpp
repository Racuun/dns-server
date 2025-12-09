#include "dnslib/builder/PacketBuilder.hpp"

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


    PacketBuilder& PacketBuilder::addQuestion(std::string name, QTYPE type) {
        packet.questions.push_back(DNSQuestion(name, type, QCLASS::IN));
        packet.header.qdCount++;
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