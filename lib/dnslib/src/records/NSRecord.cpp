#include "records/NSRecord.hpp"

#include "utils/utils.hpp"
#include <vector>
#include <sstream>

namespace dnslib {

    void NSRecord::serialize(std::vector<uint8_t>& buff) const {
        ResourceRecord::serialize(buff);

        std::vector<uint8_t> tmp;
        utils::writeDomain(tmp, ns);

        utils::writeU16(buff, tmp.size());
        buff.insert(buff.end(), tmp.begin(), tmp.end());
    }

    std::string NSRecord::toString() const {
        std::stringstream ss;
        ss << name << " " << ttl << " IN NS " << ns;
        return ss.str();
    }

}