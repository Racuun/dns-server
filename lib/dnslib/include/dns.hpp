/**
 * @file dns.hpp
 * @brief A convenience header that includes all major components of the dnslib library.
 * @version 1.0
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 * @section description
 * This header is provided for ease of use. By including this single file, developers
 * can get access to all the core functionality of the DNS library, including packet
 * building, parsing, and data structures. For more granular control over included
 * code, it is recommended to include the specific headers directly.
 */

#pragma once

#include "utils/utils.hpp"
#include "utils/read.hpp"

#include "records/ResourceRecord.hpp"
#include "records/ARecord.hpp"
#include "message/DNSHeader.hpp"
#include "message/DNSQuestion.hpp"
#include "message/DNSPacket.hpp"
#include "builder/PacketBuilder.hpp"
#include "parser/RecordFactory.hpp"
#include "parser/PacketParser.hpp"

#include "message/DNSMessage.hpp"
