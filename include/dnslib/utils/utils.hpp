#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace dnslib::utils {

    void writeU8(std::vector<uint8_t>& buffer, uint8_t value);

    void writeU16(std::vector<uint8_t>& buffer, uint16_t value);

    void writeU32(std::vector<uint8_t>& buffer, uint32_t value);

    void writeDomain(std::vector<uint8_t>& buffer, std::string name);

}