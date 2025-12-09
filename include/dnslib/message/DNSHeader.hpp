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

        void setResponse(bool r);
        void setQuery(bool q);
        void setAuthoritative(bool a);
        void setTruncated(bool t);
        void setRecursionDesired(bool rd);
        void setRecursionAvailable(bool ra);
        void setZ(bool z);
        void setAuthenticData(bool ad);
        void setCheckingDisabled(bool cd);
        void setOpcode(OPCODE op);
        void setRcode(RCODE rc);

    public:
        DNSHeader() = default;

        void serialize(std::vector<uint8_t>& buff) const override;
        std::string toString() const override;

        bool isRespone() const;
        bool isQuery() const;
        bool authAns() const;
        bool truncation() const;
        bool recursionDesired() const;
        bool recursionAvailable() const;
        bool z() const;
        bool authenticData() const;
        bool checkingDisabled() const;
        OPCODE opcode() const;
        RCODE rcode() const;
    };

}