#include "dns.hpp"
#include "utils/etsqueue.hpp"
#include "utils/log.hpp"
#include "cache.hpp"
#include "message/DNSHeader.hpp"
#include <exception>
#include <iostream>


static std::unordered_map<uint16_t, sockaddr_in> pending_queries;

inline void resolverWorker(
    utils::ETSQueue<dnslib::DNSMessageL>& inputQueue, 
    utils::ETSQueue<dnslib::DNSMessageL>& outputQueue,
    TLRUCache& dnsCache
) {
    //preassemble of google dns adress
    sockaddr_in googleAddr{};
    googleAddr.sin_family = AF_INET;
    googleAddr.sin_port = htons(53);
    inet_pton(AF_INET, "8.8.8.8", &googleAddr.sin_addr);
    
    while (true) {

        dnslib::DNSMessageL message = inputQueue.pop();

        try {

            // Resolver code here

            
            // What should be done
            // 1. parse buffored 
            dnslib::PacketParser parser;
            dnslib::DNSPacket packet = parser.parse(message.data);
            uint16_t dnsId = packet.getHeader().getId();

            // 2. If pakcet is response - save it to use for responses to hosts
            bool isRequest = (message.data[2] & 0x80) == 0;
            // 3. If packet is request - send questions
            if(isRequest) {
                //Request handler
                auto questions = packet.getQuestions();
               
                if(!questions.empty()){
                    auto& first_question = questions[0]; 

                    cacheKey key;
                    key.name = first_question.getName();
                    key.type = first_question.getType();
                    
                    //Czy wywołujemy gdzieś ten cache wgl?
                    auto cached_response = dnsCache.get(key);

                    //Hit in cache
                    // 4. If address is known serialize and put to queue
                    if(cached_response.has_value()){
                        LOG_INFO("Cache HIT for: " + key.name);

                        dnslib::PacketBuilder builder;
                        builder.setId(packet.getHeader().getId());
                        builder.withFlags(dnslib::PacketFlag::RESPONSE);
                        builder.addQuestion(first_question.getName(), first_question.getType());
                        
                        for (const auto& record : *cached_response.value()) {
                            builder.addAnswer(record);
                        }

                        dnslib::DNSPacket response_packet = builder.build();

                        message.data.clear();
                        response_packet.serialize(message.data);

                        outputQueue.push(std::move(message)); 
                                            
                    }

                    //MISS in cache
                    else{
                        LOG_INFO("Cache MISS: " + key.name + " -> Forwarding to 8.8.8.8");

                        pending_queries[dnsId] = message.peerAddress;

                        message.peerAddress = googleAddr;
                        outputQueue.push(std::move(message));
                    }
                }
            }

            else{
                //Response handler
                if (pending_queries.count(dnsId)){

                    auto answers = packet.getAnswers();
                    auto questions = packet.getQuestions();

                    if (!answers.empty() && !questions.empty()) {
                        auto& first_question = questions[0];

                        auto cacheVec = std::make_shared<std::vector<std::shared_ptr<dnslib::ResourceRecord>>>(answers);
                        
                        uint32_t ttl = answers[0]->getTtl();
                        
                        cacheKey key;
                        key.name = first_question.getName();
                        key.type = first_question.getType();

                        // Zapisujemy w cache
                        dnsCache.put(key, cacheVec, ttl);
                    }

                    message.peerAddress = pending_queries[dnsId];
                    
                    // Usuwamy wpis z mapy (transakcja zakończona)
                    pending_queries.erase(dnsId);

                    // Wysyłamy odpowiedź do klienta
                    outputQueue.push(std::move(message));

                }
                else {
                    // Ignorujemy pakiety, których nie zamawialiśmy
                    LOG_WARN("Zignorowano nieznaną odpowiedź ID: " + std::to_string(dnsId));
                }
            }

           

            // Don't forget to check cache - ok

            //outputQueue.push(std::move(output));

        } catch (const std::exception& e) {
            LOG_ERR("Error " + std::string(e.what()));
        }
    }

}