#include "dns.hpp"
#include "utils/etsqueue.hpp"
#include "utils/log.hpp"
#include "cache.hpp"
#include "message/DNSHeader.hpp"
#include <exception>
#include <iostream>
#include <random>
#include <arpa/inet.h>

// Root DNS servers (a subset for simplicity)
static const char* ROOT_SERVERS[] = {
    "198.41.0.4",      // a.root-servers.net
    "199.9.14.201",    // b.root-servers.net
    "192.33.4.12",     // c.root-servers.net
    "199.7.91.13",     // d.root-servers.net
    "192.203.230.10",  // e.root-servers.net
};

static const int ROOT_SERVER_COUNT = 5;

// Helper function to create sockaddr_in from IP string
static sockaddr_in makeAddress(const std::string& ip) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(53);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    return addr;
}

// Send query and receive response
static std::vector<uint8_t> queryServer(const sockaddr_in& server, const std::vector<uint8_t>& query, int sockfd) {
    // Send query
    sendto(sockfd, query.data(), query.size(), 0, 
           (sockaddr*)&server, sizeof(server));
    
    // Receive response with timeout
    std::vector<uint8_t> buffer(512);
    sockaddr_in fromAddr{};
    socklen_t fromLen = sizeof(fromAddr);
    
    // Set receive timeout
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    ssize_t received = recvfrom(sockfd, buffer.data(), buffer.size(), 0,
                                (sockaddr*)&fromAddr, &fromLen);
    
    if (received > 0) {
        buffer.resize(received);
        return buffer;
    }
    
    return std::vector<uint8_t>();
}

// Recursive resolve function
static std::vector<std::shared_ptr<dnslib::ResourceRecord>> resolveRecursive(
    const std::string& domain,
    dnslib::TYPE qtype,
    TLRUCache& cache,
    int sockfd,
    int depth = 0
) {
    const int MAX_DEPTH = 10;
    if (depth > MAX_DEPTH) {
        LOG_WARN("Max recursion depth reached for: " + domain);
        return {};
    }
    
    // Check cache first
    cacheKey key{domain, qtype};
    auto cached = cache.get(key);
    if (cached.has_value()) {
        LOG_DEBUG("Cache HIT at depth " + std::to_string(depth) + ": " + domain);
        return *cached.value();
    }
    
    LOG_DEBUG("Resolving at depth " + std::to_string(depth) + ": " + domain);
    
    // Start from a random root server
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, ROOT_SERVER_COUNT - 1);
    
    std::vector<std::string> serversToQuery;
    serversToQuery.push_back(ROOT_SERVERS[dist(gen)]);
    
    // Iteratively query nameservers
    for (int iteration = 0; iteration < 10; iteration++) {
        if (serversToQuery.empty()) {
            LOG_WARN("No more servers to query for: " + domain);
            break;
        }
        
        std::string currentServer = serversToQuery.back();
        serversToQuery.pop_back();
        
        LOG_DEBUG("Querying " + currentServer + " for " + domain);
        
        // Build query
        dnslib::PacketBuilder builder;
        builder.setId(rand() % 65536);
        builder.withFlags(dnslib::PacketFlag::RECURSION_DES);
        builder.addQuestion(domain, qtype);
        
        std::vector<uint8_t> queryData;
        builder.build().serialize(queryData);
        
        // Send query
        sockaddr_in serverAddr = makeAddress(currentServer);
        std::vector<uint8_t> response = queryServer(serverAddr, queryData, sockfd);
        
        if (response.empty()) {
            LOG_DEBUG("No response from " + currentServer);
            continue;
        }
        
        // Parse response
        try {
            dnslib::DNSPacket packet = dnslib::PacketParser::parse(response);
            auto answers = packet.getAnswers();
            
            // Got answer?
            if (!answers.empty()) {
                LOG_INFO("Got answer for " + domain + " from " + currentServer);
                
                // Cache the result
                auto cacheVec = std::make_shared<std::vector<std::shared_ptr<dnslib::ResourceRecord>>>(answers);
                uint32_t ttl = answers[0]->getTtl();
                cache.put(key, cacheVec, ttl);
                
                // Handle CNAME by recursively resolving
                if (answers[0]->getType() == static_cast<uint16_t>(dnslib::TYPE::CNAME)) {
                    auto cnameRecord = std::dynamic_pointer_cast<dnslib::CNAMERecord>(answers[0]);
                    if (cnameRecord && qtype == dnslib::TYPE::A) {
                        std::string cname = cnameRecord->getCname();
                        LOG_DEBUG("Following CNAME: " + domain + " -> " + cname);
                        return resolveRecursive(cname, qtype, cache, sockfd, depth + 1);
                    }
                }
                
                return answers;
            }
            
            // No answer - look for NS records to continue
            // In a full implementation, we'd parse authority section
            // For simplicity, we'll just fail here
            LOG_DEBUG("No answer from " + currentServer + ", response has " + 
                     std::to_string(answers.size()) + " answers");
            
        } catch (const std::exception& e) {
            LOG_WARN("Error parsing response: " + std::string(e.what()));
        }
    }
    
    LOG_WARN("Failed to resolve: " + domain);
    return {};
}

inline void resolverWorker(
    utils::ETSQueue<dnslib::DNSMessageL>& inputQueue, 
    utils::ETSQueue<dnslib::DNSMessageL>& outputQueue,
    TLRUCache& dnsCache
) {
    // Create a socket for recursive queries
    int querySock = socket(AF_INET, SOCK_DGRAM, 0);
    if (querySock < 0) {
        LOG_ERR("Failed to create query socket");
        return;
    }
    
    while (true) {
        dnslib::DNSMessageL message = inputQueue.pop();
        
        try {
            // Parse incoming request
            dnslib::DNSPacket packet = dnslib::PacketParser::parse(message.data);
            uint16_t dnsId = packet.getHeader().getId();
            
            // Only handle requests
            bool isRequest = (message.data[2] & 0x80) == 0;
            if (!isRequest) {
                LOG_WARN("Received unexpected response, ignoring");
                continue;
            }
            
            auto questions = packet.getQuestions();
            if (questions.empty()) {
                LOG_WARN("Request with no questions");
                continue;
            }
            
            auto& question = questions[0];
            std::string qname = question.getName();
            dnslib::TYPE qtype = question.getType();
            
            LOG_INFO("Request: " + qname + " " + std::to_string(qtype));
            
            // Perform recursive resolution
            auto answers = resolveRecursive(qname, qtype, dnsCache, querySock);
            
            // Build response
            dnslib::PacketBuilder builder;
            builder.setId(dnsId);
            builder.withFlags(dnslib::PacketFlag::RESPONSE | 
                            dnslib::PacketFlag::RECURSION_DES | 
                            dnslib::PacketFlag::RECURSION_AVAIL);
            builder.addQuestion(qname, qtype);
            
            if (answers.empty()) {
                builder.withRcode(dnslib::RCODE::NAMEERROR);
            } else {
                builder.withRcode(dnslib::RCODE::NOERROR);
                for (const auto& record : answers) {
                    builder.addAnswer(record);
                }
            }
            
            dnslib::DNSPacket responsePacket = builder.build();
            message.data.clear();
            responsePacket.serialize(message.data);
            
            outputQueue.push(std::move(message));
            
        } catch (const std::exception& e) {
            LOG_ERR("Error: " + std::string(e.what()));
        }
    }
    
    close(querySock);
}