/**
 * @file RecordFactory.hpp
 * @brief Defines the RecordFactory class for creating ResourceRecord objects from raw DNS data.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "dnslib/records/ResourceRecord.hpp"
#include "dnslib/utils/read.hpp"
#include <memory>

namespace dnslib {
    class DNSPacket;

    /**
     * @brief A factory class for creating specific ResourceRecord instances from a byte stream.
     * 
     * This factory is responsible for parsing the common header of a resource record,
     * determining its type (e.g., A, AAAA, CNAME), and then instantiating the
     * corresponding concrete ResourceRecord class to parse the record-specific data (RDATA).
     */
    class RecordFactory {
    public:
        /**
         * @brief Creates a ResourceRecord by parsing data from a ByteReader.
         * 
         * This static method reads the record's name, type, class, TTL, and RDLENGTH.
         * Based on the record type, it instantiates the appropriate concrete subclass
         * (like ARecord) and returns a shared pointer to it. The reader's position
         * will be advanced past the parsed record.
         * 
         * @param reader A ByteReader instance positioned at the beginning of a resource record.
         * @return A std::shared_ptr<ResourceRecord> pointing to the newly created record object.
         *         Returns a nullptr if the record type is unknown or not supported.
         */
        static std::shared_ptr<ResourceRecord> create(utils::ByteReader& reader);
    };
}