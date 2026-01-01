#include "message/DNSHeader.hpp"
#include "utils/utils.hpp"
#include <cstdint>
#include <string>


namespace dnslib {

    void DNSHeader::serialize(std::vector<uint8_t>& buff) const {
        utils::writeU16(buff, id);
        utils::writeU16(buff, flags);
        utils::writeU16(buff, qdCount);
        utils::writeU16(buff, anCount);
        utils::writeU16(buff, nsCount);
        utils::writeU16(buff, arCount);
    }    
    
    std::string DNSHeader::toString() const {
        return "id: " + std::to_string(id) + 
            "\nflags: " + std::to_string(flags) +
            "\nqdcount: " + std::to_string(qdCount) + 
            "\nancount: " + std::to_string(anCount) + 
            "\nnscount: " + std::to_string(nsCount) + 
            "\narcount: " + std::to_string(arCount);
    }

}