#include <gtest/gtest.h>
#include "dns.hpp"

using namespace dnslib;

TEST(DnsQuestionTest, CreateQuestion) {
    DNSQuestion question("google.com", TYPE::A, CLASS::IN);
    EXPECT_EQ(question.getName(), "google.com");
    EXPECT_EQ(question.getType(), TYPE::A);
    EXPECT_EQ(question.getClass(), CLASS::IN);
}

TEST(DnsQuestionTest, EncodeQuestion) {
    DNSQuestion question("google.com", TYPE::A, CLASS::IN);
    std::vector<uint8_t> buffer;
    question.serialize(buffer);

    // Expected encoding for "google.com" is 6google3com0
    std::vector<uint8_t> expected_buffer = {
        6, 'g', 'o', 'o', 'g', 'l', 'e',
        3, 'c', 'o', 'm',
        0,
        0, 1, // TYPE
        0, 1  // CLASS
    };

    EXPECT_EQ(buffer, expected_buffer);
}
