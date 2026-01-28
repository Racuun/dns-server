#include <gtest/gtest.h>
#include "dns.hpp"

using namespace dnslib;

TEST(UnknownRecordTest, CreateUnknownRecord) {
    std::vector<uint8_t> data = {0xDE, 0xAD, 0xBE, 0xEF};
    UnknownRecord record("unknown.com", 999, 3600, data);
    
    EXPECT_EQ(record.getName(), "unknown.com");
    EXPECT_EQ(record.getType(), 999);
    EXPECT_EQ(record.getTtl(), 3600);
    EXPECT_EQ(record.getRdata(), data);
}

TEST(UnknownRecordTest, SerializeUnknownRecord) {
    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    UnknownRecord record("unknown.com", 999, 3600, data);
    std::vector<uint8_t> buffer;
    record.serialize(buffer);

    std::vector<uint8_t> expected_buffer = {
        // Name: unknown.com
        7, 'u', 'n', 'k', 'n', 'o', 'w', 'n', 3, 'c', 'o', 'm', 0,
        // Type: 999 (0x03E7)
        0x03, 0xE7,
        // Class: IN (1)
        0, 1,
        // TTL: 3600
        0, 0, 0x0E, 0x10,
        // RDLENGTH: 3
        0, 3,
        // RDATA
        1, 2, 3
    };

    EXPECT_EQ(buffer, expected_buffer);
}

TEST(UnknownRecordTest, ToStringUnknownRecord) {
    std::vector<uint8_t> data = {0xAB, 0xCD};
    UnknownRecord record("unknown.com", 999, 3600, data);
    
    // Expected format: NAME TTL IN TYPE<TYPE> \# <LEN> ( <HEX_DATA> )
    // Note: The hex output format in UnknownRecord.cpp uses uppercase and zero padding.
    // "unknown.com 3600 IN TYPE999 \# 2 ( AB CD )"
    std::string expected = "unknown.com 3600 IN TYPE999 \\# 2 ( AB CD )";
    EXPECT_EQ(record.toString(), expected);
}
