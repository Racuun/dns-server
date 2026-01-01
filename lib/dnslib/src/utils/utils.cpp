#include "utils/utils.hpp"
#include <stdexcept>

namespace dnslib::utils {

    void writeU8(std::vector<uint8_t>& buffer, uint8_t value) {
        buffer.push_back(value);
    }

    void writeU16(std::vector<uint8_t>& buffer, uint16_t value) {
        buffer.push_back(value >> 8);
        buffer.push_back(value & 0xFF);
    }

    void writeU32(std::vector<uint8_t>& buffer, uint32_t value) {
        buffer.push_back((value >> 24) & 0xFF);
        buffer.push_back((value >> 16) & 0xFF);
        buffer.push_back((value >> 8) & 0xFF);
        buffer.push_back(value & 0xFF);
    }

    void writeDomain(std::vector<uint8_t>& buffer, std::string name) {
        if (!name.empty() && name.back() == '.') {
            name.pop_back();
        }

        size_t start = 0;
        size_t end = 0;

        while ((end = name.find('.', start)) != std::string::npos) {
            size_t len = end - start;
            
            if (len > 63) {
                 throw std::runtime_error("Label too long");
            }

            buffer.push_back(static_cast<uint8_t>(len));

            for (size_t i = 0; i < len; ++i) {
                buffer.push_back(name[start + i]);
            }

            start = end + 1;
        }

        if (start < name.length()) {
            size_t len = name.length() - start;
             if (len > 63) throw std::runtime_error("Label too long");

            buffer.push_back(static_cast<uint8_t>(len));
            for (size_t i = 0; i < len; ++i) {
                buffer.push_back(name[start + i]);
            }
        }

        buffer.push_back(0x00);
    }

}