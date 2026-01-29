#include "parser/RecordFactory.hpp"
#include "records/_records.hpp"
#include <cstddef>
#include <cstdint>

namespace dnslib {

    std::shared_ptr<ResourceRecord> RecordFactory::create(utils::ByteReader& reader) {
        std::string name = reader.readDomain();
        TYPE type = static_cast<TYPE>(reader.readU16());
        reader.readU16(); // skip class
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
            case TYPE::NS: {
                std::string ns = reader.readDomain();
                record = std::make_shared<NSRecord>(name, ttl, ns);
                break;
            }
            case TYPE::CNAME: {
                std::string cname = reader.readDomain();
                record = std::make_shared<CNAMERecord>(name, ttl, cname);
                break;
            }
            case TYPE::PTR: {
                std::string ptr = reader.readDomain();
                record = std::make_shared<PTRRecord>(name, ttl, ptr);
                break;
            }
            case TYPE::MX: {
                std::uint16_t pref = reader.readU16();
                std::string exch = reader.readDomain();
                record = std::make_shared<MXRecord>(name, ttl, pref, exch);
                break;
            }
            default: {
                std::vector<uint8_t> data;
                data.reserve(rdlength);
                for (int i = 0; i < rdlength; ++i) {
                    data.push_back(reader.readU8());
                }
                record = std::make_shared<UnknownRecord>(name, static_cast<uint16_t>(type), ttl, data);
                break;
            }
        }

        reader.setPosition(expectedEnd);
        return record;
    }
}