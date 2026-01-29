#include "dns.hpp"
#include "message/DNSPacket.hpp"
#include "utils/etsqueue.hpp"
#include "utils/log.hpp"
#include "cache.hpp"
#include <exception>
#include <iostream>
#include <arpa/inet.h> 


auto CheckCache(auto questions, auto& packet, TLRUCache& dnsCache) -> std::optional<dnslib::DNSPacket> {
    
    if (questions.empty()) return std::nullopt;

    auto& first_question = questions[0]; 
    
    cacheKey key;
    key.name = first_question.getName();
    key.type = first_question.getType();
    
    //Czy wywołujemy gdzieś ten cache wgl?
    auto cached_response = dnsCache.get(key);
    
    //Hit in cache
    // 4. If address is known serialize and put to queue
    if(cached_response.has_value()){
        DNS_LOG_INFO("Cache HIT for: " + key.name);
    
        dnslib::PacketBuilder builder;
        builder.setId(packet.getHeader().getId());
        
        dnslib::PacketFlag flags = dnslib::PacketFlag::RESPONSE | dnslib::PacketFlag::RECURSION_AVAIL;
        if (packet.getHeader().recursionDesired()) {
            flags = flags | dnslib::PacketFlag::RECURSION_DES;
        }
        builder.withFlags(flags);
        
        builder.addQuestion(first_question.getName(), first_question.getType());
        
        for (const auto& record : *cached_response.value()) {
            builder.addAnswer(record);
        }
    
        dnslib::DNSPacket response_packet = builder.build();
    
        return response_packet;
    }

    return std::nullopt;
}

struct PendingQuery {
    sockaddr_in address;
    bool recursionDesired;
};

static std::unordered_map<uint16_t, PendingQuery> pending_queries;

inline void HandleRequest(
    dnslib::DNSMessageL& message,
    dnslib::DNSPacket& packet,
    utils::ETSQueue<dnslib::DNSMessageL>& outputQueue,
    TLRUCache& dnsCache,
    const sockaddr_in& upstreamAddr
) {
    auto questions = packet.getQuestions();
    if (questions.empty()) return;

    uint16_t dnsId = packet.getHeader().getId();
    DNS_LOG_DEBUG("Received Request ID: " + std::to_string(dnsId) + " for " + questions[0].getName());

    auto response_packet = CheckCache(questions, packet, dnsCache);

    if (!response_packet.has_value()) {
        DNS_LOG_INFO("Cache MISS: " + questions[0].getName() + " -> Requesting recursive...");
        
        pending_queries[dnsId] = {message.peerAddress, packet.getHeader().recursionDesired()};
        message.peerAddress = upstreamAddr;

        // Clear RD flag for iterative query to Root
        if (message.data.size() >= 3) {
            message.data[2] &= ~0x01;
        }
        outputQueue.push(std::move(message));
    } else {
        message.data.clear();
        response_packet->serialize(message.data);
        outputQueue.push(std::move(message));
    }
}

inline void HandleResponse(
    dnslib::DNSMessageL& message,
    dnslib::DNSPacket& packet,
    utils::ETSQueue<dnslib::DNSMessageL>& outputQueue,
    TLRUCache& dnsCache
) {
    uint16_t dnsId = packet.getHeader().getId();
    DNS_LOG_DEBUG("Received Response ID: " + std::to_string(dnsId));

    if (pending_queries.count(dnsId) == 0) {
        DNS_LOG_WARN("Zignorowano nieznaną odpowiedź ID: " + std::to_string(dnsId));
        return;
    }

    auto answers = packet.getAnswers();
    auto questions = packet.getQuestions();
    auto authority = packet.getAuthority();
    auto additional = packet.getAdditional();

    // Referral handling
    if (answers.empty() && !authority.empty() && !additional.empty()) {
        for(const auto& auth_rec : authority) {
            if(auth_rec->getType() != static_cast<uint16_t>(dnslib::TYPE::NS)) continue;
            
            auto ns_rec = std::dynamic_pointer_cast<dnslib::NSRecord>(auth_rec);
            if(!ns_rec) continue;
            
            std::string ns_name = ns_rec->getNs();
            
            for(const auto& add_rec : additional) {
                if(add_rec->getType() != static_cast<uint16_t>(dnslib::TYPE::A)) continue;
                if(add_rec->getName() != ns_name) continue;

                auto a_rec = std::dynamic_pointer_cast<dnslib::ARecord>(add_rec);
                if(!a_rec) continue;
                
                DNS_LOG_INFO("Referral: " + questions[0].getName() + " -> " + ns_name);
                
                sockaddr_in next_addr{};
                next_addr.sin_family = AF_INET;
                next_addr.sin_port = htons(53);
                next_addr.sin_addr.s_addr = htonl(a_rec->getIpAddress());
                
                message.peerAddress = next_addr;
                
                dnslib::PacketBuilder builder;
                builder.setId(dnsId);
                builder.withFlags(dnslib::PacketFlag::NONE);
                builder.addQuestion(questions[0].getName(), questions[0].getType());
                
                dnslib::DNSPacket query_pkt = builder.build();
                message.data.clear();
                query_pkt.serialize(message.data);
                
                outputQueue.push(std::move(message));
                return;
            }
        }
    }

    // Cache handling
    if (!answers.empty() && !questions.empty()) {
        auto& first_question = questions[0];
        auto cacheVec = std::make_shared<std::vector<std::shared_ptr<dnslib::ResourceRecord>>>(answers);
        uint32_t ttl = answers[0]->getTtl();
        
        cacheKey key;
        key.name = first_question.getName();
        key.type = first_question.getType();
        dnsCache.put(key, cacheVec, ttl);
    }

    // Final response
    auto& pending = pending_queries[dnsId];
    message.peerAddress = pending.address;
    
    if (message.data.size() >= 4) {
        message.data[3] |= 0x80;
        if (pending.recursionDesired) {
            message.data[2] |= 0x01;
        } else {
            message.data[2] &= ~0x01;
        }
    }

    pending_queries.erase(dnsId);
    DNS_LOG_DEBUG("Sending final response to client for ID: " + std::to_string(dnsId));
    outputQueue.push(std::move(message));
}

inline void resolverWorker(
    utils::ETSQueue<dnslib::DNSMessageL>& inputQueue, 
    utils::ETSQueue<dnslib::DNSMessageL>& outputQueue,
    TLRUCache& dnsCache
) {
    //preassemble of google dns adress
    sockaddr_in rootAddr{};
    rootAddr.sin_family = AF_INET;
    rootAddr.sin_port = htons(53);
    inet_pton(AF_INET, "199.7.83.42", &rootAddr.sin_addr);
    
    while (true) {

        dnslib::DNSMessageL message = inputQueue.pop();

        try {

            dnslib::PacketParser parser;
            dnslib::DNSPacket packet = parser.parse(message.data);

            bool isRequest = (message.data[2] & 0x80) == 0;
            
            if(isRequest) {
                HandleRequest(message, packet, outputQueue, dnsCache, rootAddr);
            }
            else{
                HandleResponse(message, packet, outputQueue, dnsCache);
            }

        } catch (const std::exception& e) {
            DNS_LOG_ERR("Error " + std::string(e.what()));
        }
    }

}