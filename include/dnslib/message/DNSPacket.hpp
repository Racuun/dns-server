#pragma once

#include "DNSHeader.hpp"
#include "DNSQuestion.hpp"
#include "../records/ResourceRecord.hpp"
#include <memory>

namespace dnslib {

    class PacketBuilder;

    class DNSPacket : public ISerializable {
        friend class PacketBuilder;
    private:
        DNSHeader header;
        std::vector<DNSQuestion> questions;
        
        std::vector<std::shared_ptr<ResourceRecord>> answers;
    public:
        DNSPacket() = default;

        void serialize(std::vector<uint8_t>& buff) const override;
        std::string toString() const override;
    };

}