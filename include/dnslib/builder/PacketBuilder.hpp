#pragma once

#include "../message/DNSPacket.hpp"
#include <cstddef>
#include <cstdint>

namespace dnslib {

    enum class PacketFlag : uint16_t {
        NONE            = 0,
        RESPONSE        = 1 << 15,
        AUTHORITATIVE   = 1 << 10,
        TRUNCATED       = 1 << 9,
        RECURSION_DES   = 1 << 8,
        RECURSION_AVAIL = 1 << 7 
    };

    inline PacketFlag operator|(PacketFlag a, PacketFlag b) {
        return static_cast<PacketFlag>(
            static_cast<uint16_t>(a) | static_cast<uint16_t>(b)
        );
    }

    inline bool operator&(PacketFlag a, PacketFlag b) {
        return (static_cast<uint16_t>(a) & static_cast<uint16_t>(b)) != 0;
    }


    class PacketBuilder {
        DNSPacket packet;
    public:
        PacketBuilder() = default;
        PacketBuilder(uint16_t id);

        // configuration
        PacketBuilder& setId(uint16_t id);
        PacketBuilder& withOpcode(OPCODE op);
        PacketBuilder& withRcode(RCODE rc);
        PacketBuilder& withFlags(PacketFlag flags);

        PacketBuilder& addQuestion(std::string name, QTYPE type);
        
        // utility
        PacketBuilder& expectedAnswers(size_t count);

        DNSPacket build();
    };
}