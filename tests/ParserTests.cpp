#include <gtest/gtest.h>
#include <arpa/inet.h>
#include "dnslib/dns.hpp"

using namespace dnslib;

class DnsPacketTest : public ::testing::Test {
protected:
    PacketBuilder builder;
};

TEST_F(DnsPacketTest, EncodeDecodeCycle) {
    // 1. Arrange: Zbuduj pakiet
    std::string expectedName = "google.com";
    std::string expectedIp = "8.8.8.8";
    
    auto originalPacket = builder
        .setId(0x1337)
        .withFlags(PacketFlag::RECURSION_DES) // Type A
        .addQuestion(expectedName, QTYPE::A)
        //.addARecord(expectedName, expectedIp)
        .build();

    // 2. Act: Serializacja -> Parsowanie
    std::vector<uint8_t> bytes;
    originalPacket.serialize(bytes);
    
    // Tutaj symulujemy sieć (bytes to surowe dane)
    auto parsedPacket = PacketParser::parse(bytes);

    // 3. Assert: Sprawdź czy to samo
    EXPECT_EQ(parsedPacket.getHeader().getId(), 0x1337);
    EXPECT_TRUE(parsedPacket.getHeader().recursionDesired());
    
    // Sprawdź pytanie
    ASSERT_EQ(parsedPacket.getQuestions().size(), 1);
    EXPECT_EQ(parsedPacket.getQuestions()[0].getName(), expectedName);

    // // Sprawdź odpowiedź (Polimorfizm w akcji!)
    // ASSERT_EQ(parsedPacket.getAnswers().size(), 1);
    
    // // Rzutowanie w dół, żeby sprawdzić czy Factory stworzyło dobry typ
    // auto record = parsedPacket.getAnswers()[0];
    // auto aRecord = std::dynamic_pointer_cast<ARecord>(record);
    
    // ASSERT_NE(aRecord, nullptr); // Czy to na pewno ARecord?
    // EXPECT_EQ(aRecord->getIpAddress(), inet_addr(expectedIp.c_str()));
}