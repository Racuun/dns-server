#pragma once

#include "../interface/ISerializable.hpp"

namespace dnslib {

    class PacketBuilder;
    
    enum class OPCODE : uint8_t {
        QUERY = 0,
        IQUERY = 1,
        STATUS = 2
    };

    enum class RCODE : uint8_t {
        NOERROR = 0,
        FORMATERROR = 1,
        SERVERFAILURE = 2,
        NAMEERROR = 3,
        NOTIMPLEMENTED = 4,
        REFUSED = 5
    };

    class DNSHeader : public ISerializable {
        friend class PacketBuilder;
    private:
        uint16_t id;
        uint16_t flags = 0;
        uint16_t qdCount = 0;
        uint16_t anCount = 0;
        uint16_t nsCount = 0;
        uint16_t arCount = 0;

        static constexpr uint16_t QR_MASK     = 1 << 15; 
        static constexpr uint16_t OPCODE_MASK = 0xF << 11; 
        static constexpr uint16_t AA_MASK     = 1 << 10; 
        static constexpr uint16_t TC_MASK     = 1 << 9;
        static constexpr uint16_t RD_MASK     = 1 << 8;
        static constexpr uint16_t RA_MASK     = 1 << 7;
        static constexpr uint16_t AD_MASK     = 1 << 6;
        static constexpr uint16_t CD_MASK     = 1 << 5;
        static constexpr uint16_t RCODE_MASK  = 0x000F;

        void setResponse(bool r) { flags = r ? flags | QR_MASK : flags & ~QR_MASK; }
        void setQuery(bool q) { flags = q ? flags & ~QR_MASK : flags | QR_MASK; }
        void setAuthoritative(bool a) { flags = a ? flags | AA_MASK : flags & ~AA_MASK; }
        void setTruncated(bool t) { flags = t ? flags | TC_MASK : flags & ~TC_MASK; }
        void setRecursionDesired(bool rd) { flags = rd ? flags | RD_MASK : flags & ~RD_MASK; }
        void setRecursionAvailable(bool ra) { flags = ra ? flags | RA_MASK : flags & ~RA_MASK; }
        void setZ(bool z) { flags = z ? flags | 0x0070 : flags & ~0x0070; }
        void setAuthenticData(bool ad) { flags = ad ? flags | AD_MASK : flags & ~AD_MASK; }
        void setCheckingDisabled(bool cd) { flags = cd ? flags | CD_MASK : flags & ~CD_MASK; }
        void setOpcode(OPCODE op) {
            flags &= ~OPCODE_MASK;
            flags |= (static_cast<uint16_t>(op) << 11) & OPCODE_MASK;
        }
        void setRcode(RCODE rc) {
            flags &= ~RCODE_MASK;
            flags |= static_cast<uint16_t>(rc) & RCODE_MASK;
        }

    public:
        DNSHeader() = default;

        void serialize(std::vector<uint8_t>& buff) const override;
        std::string toString() const override;

        bool isRespone() const { return flags & QR_MASK; }
        bool isQuery() const { return !isRespone(); }
        bool authAns() const { return flags & AA_MASK; }
        bool truncation() const { return flags & TC_MASK; }
        bool recursionDesired() const { return flags & RD_MASK; }
        bool recursionAvailable() const { return flags & RA_MASK; }
        bool z() const { return flags & 0x0070; }
        bool authenticData() const { return flags & AD_MASK; }
        bool checkingDisabled() const { return flags & CD_MASK; }
        OPCODE opcode() const { return static_cast<OPCODE>((flags >> 11) & OPCODE_MASK); }
        RCODE rcode() const { return static_cast<RCODE>(flags & RCODE_MASK); }
    };

}