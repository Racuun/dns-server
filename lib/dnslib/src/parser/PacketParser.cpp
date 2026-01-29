#include "parser/PacketParser.hpp"
#include "builder/PacketBuilder.hpp"
#include "parser/RecordFactory.hpp"
#include "message/DNSPacket.hpp"
#include "utils/read.hpp"


namespace dnslib {

    DNSPacket PacketParser::parse(const std::vector<uint8_t>& rawData) {
        utils::ByteReader reader(rawData);

        PacketBuilder builder;

        builder.setId(reader.readU16());

        uint16_t flags = reader.readU16();
        builder.withRawFlags(flags);

        uint16_t qdCount = reader.readU16();
        uint16_t anCount = reader.readU16();
        uint16_t auCount = reader.readU16();
        uint16_t arCount = reader.readU16();

        builder.expectedAnswers(anCount);

        for (int i=0; i<qdCount; i++) {
            std::string name = reader.readDomain();
            TYPE type = static_cast<TYPE>(reader.readU16());
            reader.readU16(); // skip class

            builder.addQuestion(name, type);
        }

        for (int i=0; i<anCount; i++) {
            auto record = RecordFactory::create(reader);
            builder.addAnswer(record);
        }

        for (int i=0; i<auCount; i++) {
            auto record = RecordFactory::create(reader);
            builder.addAuthority(record);
        }

        for (int i=0; i<arCount; i++) {
            auto record = RecordFactory::create(reader);
            builder.addAdditional(record);
        }

        return builder.build();
    }
}