#pragma once

#include <cstdint>
#include <vector>


namespace dnslib {

    class DNSPacket;

    class PacketBuilder;

    class PacketParser{
    public:
        static DNSPacket parse(const std::vector<uint8_t>& rawData);
    };

}