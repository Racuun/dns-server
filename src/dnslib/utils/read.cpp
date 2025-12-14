#include "dnslib/utils/read.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace dnslib::utils {

    void ByteReader::chceckBounds(size_t n) const {
        if (index + n > buffer.size()) {
            throw std::out_of_range("Packet truncated (unexpected end of data)");
        }
    }

    uint8_t ByteReader::readU8() {
        chceckBounds(1);
        return buffer[index++];
    }

    uint16_t ByteReader::readU16() {
        chceckBounds(2);
        uint16_t value = buffer[index];
        value <<= 8;
        value |= buffer[index+1];
        index += 2;
        return value;
    }

    uint32_t ByteReader::readU32() {
        chceckBounds(4);
        uint32_t value = buffer[index] << 24;
        value |= buffer[index+1] << 16;
        value |= buffer[index+2] << 8;
        value |= buffer[index+3];
        index += 4;
        return value;
    }

    std::string ByteReader::readDomain() {
        std::string domain = "";
        bool jumped = false;
        size_t jumpReturnPos = 0;

        int jumpsCount = 0;
        const int MAX_JUMPS = 5;

        while (true) {
            if (index >= buffer.size()) {
                throw std::runtime_error("EOF in domain name");
            }

            uint8_t len = buffer[index];

            if (len == 0) {
                index++;
                break;
            }

            if ((len & 0xC0) == 0xC0) {
                if (jumpsCount++ > MAX_JUMPS) {
                    throw std::runtime_error("Loop detected - Too many jumps in domain name");
                }

                chceckBounds(2);

                uint16_t offset = ((buffer[index] & 0x3F) << 8) | buffer[index+1];
                
                if (!jumped) {
                    jumpReturnPos = index+2;
                }

                index = offset;
                jumped = true;
                continue;
            }

            index++;
            chceckBounds(len);

            if (!domain.empty()) {
                domain += '.';
            }

            for (int i=0; i<len; i++) {
                domain += buffer[index++];
            }   
        }

        if (jumped) {
            index = jumpReturnPos;
        }

        return domain;
    }

}