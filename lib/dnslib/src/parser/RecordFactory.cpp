#include "parser/RecordFactory.hpp"
#include "records/ARecord.hpp"
#include "records/ResourceRecord.hpp"
#include <cstddef>
#include <cstdint>

namespace dnslib {

    std::shared_ptr<ResourceRecord> RecordFactory::create(utils::ByteReader& reader) {
        std::string name = reader.readDomain();
        RTYPE type = static_cast<RTYPE>(reader.readU16());
        uint16_t rclass = reader.readU16();
        uint32_t ttl = reader.readU32();
        uint16_t rdlength = reader.readU16();

        size_t expectedEnd = reader.position() + rdlength;

        std::shared_ptr<ResourceRecord> record = nullptr;

        switch (type) {
            case RTYPE::A: {
                std::uint32_t ip = reader.readU32();
                record = std::make_shared<ARecord>(name, ttl, ip);
                break;
            }
            case RTYPE::MX: {
                break;
            }
            default:
                break;
        }

        reader.setPosition(expectedEnd);
        return record;
    }
}