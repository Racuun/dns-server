#include "records/ARecord.hpp"
#include "utils/utils.hpp"
#include <arpa/inet.h>
#include <sstream>
#include <netinet/in.h>


namespace dnslib {

    void ARecord::serialize(std::vector<uint8_t>& buff) const {
        // Name is not serialized as part of the record data, but rather
        // as part of the question or answer section, including pointers.
        // This is a simplified serialization.
        // utils::writeDomain(buff, name);

        // Type
        utils::writeU16(buff, type);
        // Class
        utils::writeU16(buff, 1); // IN
        // TTL
        utils::writeU32(buff, ttl);
        // RDLENGTH
        utils::writeU16(buff, 4);
        // RDATA
        utils::writeU32(buff, ipAdress);
    }

    std::string ARecord::toString() const {
        std::stringstream ss;
        in_addr addr;
        addr.s_addr = ipAdress;
        ss << "A Record: " << name << " " << ttl << " IN A " << inet_ntoa(addr);
        return ss.str();
    }

}
