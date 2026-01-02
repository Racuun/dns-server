#include <gtest/gtest.h>
#include <memory>
#include "dns.hpp"

using namespace dnslib;

TEST(BuilderTest, BuildPacket) {
    auto packet = PacketBuilder()
        .setId(123)
        .withFlags(F_RECURSION_DES | F_RECURSION_AVAIL)
        .addQuestion("google.com", TYPE::A)
        .addAnswer(std::make_shared<dnslib::ARecord>(dnslib::ARecord("google.com", 3600, "1.2.3.4")))
        .build();

    EXPECT_EQ(packet.getHeader().getId(), 123);
    EXPECT_TRUE(packet.getHeader().recursionDesired());
    EXPECT_TRUE(packet.getHeader().recursionAvailable());
    EXPECT_FALSE(packet.getHeader().isResponse());

    ASSERT_EQ(packet.getQuestions().size(), 1);
    EXPECT_EQ(packet.getQuestions()[0].getName(), "google.com");
    EXPECT_EQ(packet.getQuestions()[0].getType(), TYPE::A);

    ASSERT_EQ(packet.getAnswers().size(), 1);
    auto aRecord = std::dynamic_pointer_cast<ARecord>(packet.getAnswers()[0]);
    ASSERT_NE(aRecord, nullptr);
    EXPECT_EQ(aRecord->getName(), "google.com");
    EXPECT_EQ(aRecord->getIpAddress(), inet_addr("1.2.3.4"));
    EXPECT_EQ(aRecord->getTtl(), 3600);
}
