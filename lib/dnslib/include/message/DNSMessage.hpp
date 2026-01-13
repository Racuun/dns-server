/**
 * @file DNSMessage.hpp
 * @brief Defines the DNSMessageL struct and related PROTO enum for handling received DNS messages.
 * @version 0.1
 * @date 2026-01-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstdint>
#include <netinet/in.h>
#include <vector>
namespace dnslib {

    /**
     * @brief Specifies the transport protocol used for a DNS message.
     */
    enum class PROTO {
        UDP, ///< User Datagram Protocol. The most common protocol for DNS queries.
        TCP  ///< Transmission Control Protocol. Used for large responses or zone transfers.
    };

    /**
     * @brief Represents a DNS message received from a client, specific to a Linux environment.
     * 
     * This structure encapsulates not only the raw DNS packet data but also the
     * networking context from which it was received, such as the client's address
     * and the protocol used.
     */
    struct DNSMessageL {
        /// The raw byte data of the DNS packet.
        std::vector<uint8_t> data;
        
        /// The socket address structure of the client who sent the message.
        sockaddr_in peerAddress;

        /**
         * @brief The file descriptor of the connected client socket.
         * This is primarily relevant for TCP connections where the socket remains open.
         * For UDP, this may not be used or could represent the server's listening socket.
         */
        int clientFd;

        /// The transport protocol (UDP or TCP) over which the message was received.
        PROTO protocol;
    };
}