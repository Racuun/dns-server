#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <string>

namespace dnslib::utils {
    
    class ByteReader {
    private:
        const std::vector<uint8_t>& buffer;
        size_t index;
    
    public:
        ByteReader(const std::vector<uint8_t>& buff): buffer(buff), index(0) {}

        void chceckBounds(size_t n) const;

        uint8_t readU8();
        uint16_t readU16();
        uint32_t readU32();
        std::string readDomain();

        size_t position() const { return index; }
        void setPosition(size_t pos) { index = pos; }

        const std::vector<uint8_t>& getBuffer() const { return buffer; }

    };
}