#include "dnslib/message/DNSHeader.hpp"
#include "dnslib/utils/utils.hpp"
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
    }    std::string DNSHeader::toString() const {
        

    return "id: " + std::to_string(id) + 
            "\nflags: " + std::to_string(flags) +
            "\nqdcount: " + std::to_string(qdCount) + 
            "\nancount: " + std::to_string(anCount) + 
            "\nnscount: " + std::to_string(nsCount) + 
            "\narcount: " + std::to_string(arCount);
    }

    // Geters
    bool DNSHeader::isRespone() const { return flags & QR_MASK; }
    bool DNSHeader::isQuery() const { return !isRespone(); }
    bool DNSHeader::authAns() const { return flags & AA_MASK; }
    bool DNSHeader::truncation() const { return flags & TC_MASK; }
    bool DNSHeader::recursionDesired() const { return flags & RD_MASK; }
    bool DNSHeader::recursionAvailable() const { return flags & RA_MASK; }
    bool DNSHeader::z() const { return flags & 0x0070; }
    bool DNSHeader::authenticData() const { return flags & AD_MASK; }
    bool DNSHeader::checkingDisabled() const { return flags & CD_MASK; }
    OPCODE DNSHeader::opcode() const { return static_cast<OPCODE>((flags >> 11) & OPCODE_MASK); }
    RCODE DNSHeader::rcode() const { return static_cast<RCODE>(flags & RCODE_MASK); }

    // Seters
    void DNSHeader::setResponse(bool r) { flags = r ? flags | QR_MASK : flags & ~QR_MASK; }
    void DNSHeader::setQuery(bool q) { flags = q ? flags & ~QR_MASK : flags | QR_MASK; }
    void DNSHeader::setAuthoritative(bool a) { flags = a ? flags | AA_MASK : flags & ~AA_MASK; }
    void DNSHeader::setTruncated(bool t) { flags = t ? flags | TC_MASK : flags & ~TC_MASK; }
    void DNSHeader::setRecursionDesired(bool rd) { flags = rd ? flags | RD_MASK : flags & ~RD_MASK; }
    void DNSHeader::setRecursionAvailable(bool ra) { flags = ra ? flags | RA_MASK : flags & ~RA_MASK; }
    void DNSHeader::setZ(bool z) { flags = z ? flags | 0x0070 : flags & ~0x0070; }
    void DNSHeader::setAuthenticData(bool ad) { flags = ad ? flags | AD_MASK : flags & ~AD_MASK; }
    void DNSHeader::setCheckingDisabled(bool cd) { flags = cd ? flags | CD_MASK : flags & ~CD_MASK; }
    void DNSHeader::setOpcode(OPCODE op) {
        flags &= ~OPCODE_MASK;
        flags |= (static_cast<uint16_t>(op) << 11) & OPCODE_MASK;
    }
    void DNSHeader::setRcode(RCODE rc) {
        flags &= ~RCODE_MASK;
        flags |= static_cast<uint16_t>(rc) & RCODE_MASK;
    }


}