#include "records/CNAMERecord.hpp"
#include "utils/utils.hpp"
#include <sstream>
#include <vector>

namespace dnslib {

    void CNAMERecord::serialize(std::vector<uint8_t>& buff) const {
        ResourceRecord::serialize(buff);
        std::vector<uint8_t> tmp;
        utils::writeDomain(tmp, cname);

        utils::writeU16(buff, tmp.size());
        buff.insert(buff.end(), tmp.begin(), tmp.end());
    }

    std::string CNAMERecord::toString() const {
        std::stringstream ss;
        ss << name << " " << ttl << " IN CNAME " << cname;
        return ss.str();
    }

}