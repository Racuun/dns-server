#pragma once

#include "dnslib/records/ResourceRecord.hpp"
#include "dnslib/utils/read.hpp"
#include <memory>

namespace dnslib {
    class DNSPacket;

    class RecordFactory {
    public:
        static std::shared_ptr<ResourceRecord> create(utils::ByteReader& reader);
    };
}