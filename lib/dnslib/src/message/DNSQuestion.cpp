#include "message/DNSQuestion.hpp"
#include "utils/utils.hpp"


namespace dnslib {

    void DNSQuestion::serialize(std::vector<uint8_t>& buff) const {
        utils::writeDomain(buff, name);
        utils::writeU16(buff, type);
        utils::writeU16(buff, qclass);
    }

    std::string DNSQuestion::toString() const {
        return "name: " + name + 
            " type: " + std::to_string(getType()) + 
            " qclass: " + std::to_string(qclass);
    
    }

}