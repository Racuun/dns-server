#include "records/PTRRecord.hpp"

#include "utils/utils.hpp"
#include <sstream>

namespace dnslib {

    void PTRRecord::serialize(std::vector<uint8_t>& buff) const {
        ResourceRecord::serialize(buff);

        std::vector<uint8_t> tmp;
        utils::writeDomain(tmp, ptr);

        utils::writeU16(buff, tmp.size());
        buff.insert(buff.end(), tmp.begin(), tmp.end());
    }

    std::string PTRRecord::toString() const {
        std::stringstream ss;
        ss << name << " " << ttl << " IN PTR " << ptr;
        return ss.str();
    }

}
