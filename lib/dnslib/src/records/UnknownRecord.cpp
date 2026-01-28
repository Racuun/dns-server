#include "records/UnknownRecord.hpp"
#include "utils/utils.hpp"
#include <sstream>
#include <iomanip>

namespace dnslib {

    void UnknownRecord::serialize(std::vector<uint8_t>& buff) const {
        ResourceRecord::serialize(buff);
        
        utils::writeU16(buff, rdata.size());
        buff.insert(buff.end(), rdata.begin(), rdata.end());
    }

    std::string UnknownRecord::toString() const {
        std::stringstream ss;
        ss << name << " " << ttl << " IN TYPE" << type << " \\# " << rdata.size();
        
        if (!rdata.empty()) {
            ss << " ( ";
            for (auto b : rdata) {
                ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)b << " ";
            }
            ss << ")";
        }
        return ss.str();
    }
}
