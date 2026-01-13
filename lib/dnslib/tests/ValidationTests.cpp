#include <gtest/gtest.h>
#include "dns.hpp"
#include <arpa/inet.h>
#include <stdexcept>

using namespace dnslib;

TEST(ValidationTest, ByteReaderSetPositionOutOfBounds) {
    std::vector<uint8_t> data = {1, 2, 3};
    utils::ByteReader reader(data);
    
    // Should now throw out_of_range
    EXPECT_THROW(reader.setPosition(10), std::out_of_range);
}

TEST(ValidationTest, ARecordInvalidIP) {
    // Should now throw invalid_argument
    EXPECT_THROW(ARecord("example.com", 3600, "999.999.999.999"), std::invalid_argument);
}

TEST(ValidationTest, HeaderInvalidOpcode) {
    // This test documents current behavior regarding invalid enum casts.
    PacketBuilder pb;
    pb.withOpcode(static_cast<OPCODE>(20)); 
    
    DNSPacket pkt = pb.build();
    
    // 20 (10100) masked with 0xF (1111) is 4 (0100)
    EXPECT_EQ(static_cast<int>(pkt.getHeader().opcode()), 4);
}
