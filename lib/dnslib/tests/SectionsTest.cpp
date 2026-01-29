#include <gtest/gtest.h>
#include <memory>
#include <arpa/inet.h>
#include "dns.hpp"

using namespace dnslib;

TEST(SectionsTest, FullPacketSerialization) {
    // 1. Arrange: Build a packet with all sections
    auto packet = PacketBuilder()
        .setId(0xABCD)
        .withFlags(F_RESPONSE | F_AUTHORITATIVE)
        .addQuestion("example.com", TYPE::A)
        .addAnswer(std::make_shared<ARecord>("example.com", 300, "192.168.1.1"))
        .addAuthority(std::make_shared<NSRecord>("example.com", 86400, "ns1.example.com"))
        .addAdditional(std::make_shared<ARecord>("ns1.example.com", 300, "10.0.0.1"))
        .build();

    // Verify Builder logic
    ASSERT_EQ(packet.getHeader().getId(), 0xABCD);
    ASSERT_TRUE(packet.getHeader().isResponse());
    ASSERT_TRUE(packet.getHeader().authAns());
    
    ASSERT_EQ(packet.getQuestions().size(), 1);
    ASSERT_EQ(packet.getAnswers().size(), 1);
    ASSERT_EQ(packet.getAuthority().size(), 1);
    ASSERT_EQ(packet.getAdditional().size(), 1);

    // 2. Act: Serialize and Parse
    std::vector<uint8_t> buffer;
    packet.serialize(buffer);

    auto parsedPacket = PacketParser::parse(buffer);

    // 3. Assert: Verify parsed packet matches
    EXPECT_EQ(parsedPacket.getHeader().getId(), 0xABCD);
    EXPECT_TRUE(parsedPacket.getHeader().isResponse());
    
    // Check Question
    ASSERT_EQ(parsedPacket.getQuestions().size(), 1);
    EXPECT_EQ(parsedPacket.getQuestions()[0].getName(), "example.com");

    // Check Answer
    ASSERT_EQ(parsedPacket.getAnswers().size(), 1);
    auto ans = std::dynamic_pointer_cast<ARecord>(parsedPacket.getAnswers()[0]);
    ASSERT_NE(ans, nullptr);
    EXPECT_EQ(ans->getName(), "example.com");
    EXPECT_EQ(ans->getIpAddress(), inet_addr("192.168.1.1"));

    // Check Authority
    ASSERT_EQ(parsedPacket.getAuthority().size(), 1);
    auto auth = std::dynamic_pointer_cast<NSRecord>(parsedPacket.getAuthority()[0]);
    ASSERT_NE(auth, nullptr);
    EXPECT_EQ(auth->getName(), "example.com");
    EXPECT_EQ(auth->getNs(), "ns1.example.com");

    // Check Additional
    ASSERT_EQ(parsedPacket.getAdditional().size(), 1);
    auto add = std::dynamic_pointer_cast<ARecord>(parsedPacket.getAdditional()[0]);
    ASSERT_NE(add, nullptr);
    EXPECT_EQ(add->getName(), "ns1.example.com");
    EXPECT_EQ(add->getIpAddress(), inet_addr("10.0.0.1"));
}
