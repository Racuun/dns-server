#include <gtest/gtest.h>
#include <arpa/inet.h>
#include "dns.hpp"

using namespace dnslib;

// -------------
//      A
// -------------
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

// -------------
//      MX
// -------------
TEST(MXRecordTest, CreateMXRecord) {
    MXRecord record("example.com", 3600, 10, "mail.example.com");
    EXPECT_EQ(record.getName(), "example.com");
    EXPECT_EQ(record.getTtl(), 3600);
    EXPECT_EQ(record.getPreference(), 10);
    EXPECT_EQ(record.getExchange(), "mail.example.com");
}

TEST(MXRecordTest, ToStringMXRecord) {
    MXRecord record("example.com", 3600, 10, "mail.example.com");
    std::string expected = "example.com 3600 IN MX 10 mail.example.com";
    EXPECT_EQ(record.toString(), expected);
}

TEST(MXRecordTest, SerializeMXRecord) {
    MXRecord record("example.com", 3600, 10, "mail.example.com");
    std::vector<uint8_t> buffer;
    record.serialize(buffer);

    std::vector<uint8_t> expected_buffer = {
        // Name: example.com
        7, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        3, 'c', 'o', 'm',
        0,
        // Type: MX (15)
        0, 15,
        // Class: IN (1)
        0, 1,
        // TTL: 3600
        0, 0, 0x0E, 0x10,
        // RDLENGTH: 20 (2 bytes pref + 18 bytes exchange)
        0, 20,
        // Preference: 10
        0, 10,
        // Exchange: mail.example.com
        4, 'm', 'a', 'i', 'l',
        7, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        3, 'c', 'o', 'm',
        0
    };

    EXPECT_EQ(buffer, expected_buffer);
}
