#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace dnslib {

    class ISerializable {
    public:
        virtual ~ISerializable() = default;
        virtual void serialize(std::vector<uint8_t>& buff) const = 0;
        virtual std::string toString() const = 0;
    };

}