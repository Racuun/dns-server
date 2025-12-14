#include <gtest/gtest.h>
#include "dnslib/dns.hpp"

using namespace dnslib;

// Test sprawdzający czy domyślne wartości są zerami
TEST(DnsHeaderTest, DefaultValuesAreZero) {
    DNSHeader header;
    EXPECT_EQ(header.getId(), 0);
    EXPECT_FALSE(header.isResponse());
}

// Test sprawdzający manipulację bitami flag
TEST(DnsHeaderTest, FlagManipulationWorks) {
    DNSHeader header;
    
    // Ustawiamy flagę
    // Uwaga: Tutaj musisz użyć swojej metody, np. przez Buildera lub frienda
    // Jeśli settery są prywatne, testy jednostkowe mogą być 'friend' w klasie,
    // albo testujemy przez publiczne API (Builder).
    // Załóżmy, że na potrzeby testów masz publiczne metody lub używasz Buildera:
    
    // Sposób "White Box" (jeśli dodasz FRIEND_TEST w klasie DnsHeader):
    // header.setQr(true);
    // EXPECT_TRUE(header.isResponse());
}

// Test sprawdzający settery i gettery ID
TEST(DnsHeaderTest, SetAndGetId) {
    DNSHeader header = PacketBuilder().setId(12345).build().getHeader();
    EXPECT_EQ(header.getId(), 12345);
}