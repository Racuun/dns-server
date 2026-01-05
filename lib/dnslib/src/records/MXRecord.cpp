#include "records/MXRecord.hpp"
#include "utils/utils.hpp"
#include <vector>
#include <sstream>

namespace dnslib {

    void MXRecord::serialize(std::vector<uint8_t>& buff) const {
        ResourceRecord::serialize(buff);

        std::vector<uint8_t> tmp;
        utils::writeU16(tmp, preference);
        utils::writeDomain(tmp, exchange);

        utils::writeU16(buff, tmp.size());
        buff.insert(buff.end(), tmp.begin(), tmp.end());
    }

    std::string MXRecord::toString() const {
        std::stringstream ss;
        ss << name << " " << ttl << " IN MX " << preference << " " << exchange;
        return ss.str();
    }
}