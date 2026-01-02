#include "parser/RecordFactory.hpp"
#include "records/ARecord.hpp"
#include "records/ResourceRecord.hpp"
#include <cstddef>
#include <cstdint>

namespace dnslib {

    std::shared_ptr<ResourceRecord> RecordFactory::create(utils::ByteReader& reader) {
        std::string name = reader.readDomain();
        TYPE type = static_cast<TYPE>(reader.readU16());
        uint16_t rclass = reader.readU16();
        uint32_t ttl = reader.readU32();
        uint16_t rdlength = reader.readU16();

        size_t expectedEnd = reader.position() + rdlength;

        std::shared_ptr<ResourceRecord> record = nullptr;

        switch (type) {
            case TYPE::A: {
                std::uint32_t ip = reader.readU32();
                record = std::make_shared<ARecord>(name, ttl, ip);
                break;
            }
            case TYPE::MX: {
                break;
            }
            default:
                break;
        }

        reader.setPosition(expectedEnd);
        return record;
    }
}