#include "records/ARecord.hpp"
#include "utils/utils.hpp"
#include <arpa/inet.h>
#include <sstream>
#include <netinet/in.h>


namespace dnslib {

    void ARecord::serialize(std::vector<uint8_t>& buff) const {
        // serialize common fields
        ResourceRecord::serialize(buff);
        // RDLENGTH
        utils::writeU16(buff, 4);
        // RDATA
        utils::writeU32(buff, ipAdress);
    }

    std::string ARecord::toString() const {
        std::stringstream ss;
        in_addr addr;
        addr.s_addr = ipAdress;
        ss << name << " " << ttl << " IN A " << inet_ntoa(addr);
        return ss.str();
    }

}
