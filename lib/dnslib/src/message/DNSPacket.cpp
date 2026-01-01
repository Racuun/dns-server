#include "message/DNSPacket.hpp"
#include <string>

namespace dnslib {

    void DNSPacket::serialize(std::vector<uint8_t>& buff) const {
        header.serialize(buff);
        for (const auto& question : questions) {
            question.serialize(buff);
        }
        for (const auto& answer : answers) {
            answer->serialize(buff);
        }
    }

    std::string DNSPacket::toString() const {
        std::string log = "<< Packet LOG >>\n" + header.toString();
        if (!questions.empty()) {
            log += "\nQuestions:";
        }
        for (const auto& question : questions) {
            log += "\n  " + question.toString();
        }
        if (!answers.empty()) {
            log += "\nAnswers:";
        }
        for (const auto& answer : answers) {
            log += "\n" + answer->toString();
        }

        return log;
    }
    
}