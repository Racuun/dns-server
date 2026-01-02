#include <gtest/gtest.h>
#include <arpa/inet.h>
#include "dns.hpp"

using namespace dnslib;

TEST(ARecordTest, CreateARecord) {
    ARecord record("google.com", 12345, "8.8.8.8");
    // EXPECT_EQ(record.getName(), "google.com");
    // EXPECT_EQ(record.getTtl(), 12345);
    EXPECT_EQ(record.getIpAddress(), inet_addr("8.8.8.8"));
}

TEST(ARecordTest, EncodeARecord) {
    ARecord record("google.com", 12345, "8.8.8.8");
    std::vector<uint8_t> buffer;
    record.serialize(buffer);

    std::vector<uint8_t> expected_buffer = {
        6, 'g', 'o', 'o', 'g', 'l', 'e',
        3, 'c', 'o', 'm',
        0,
        0, 1, // TYPE
        0, 1, // CLASS
        0, 0, 0x30, 0x39, // TTL
        0, 4, // RDLENGTH
        8, 8, 8, 8 // RDATA
    };

    EXPECT_EQ(buffer, expected_buffer);
}
