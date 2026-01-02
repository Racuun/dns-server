#include "connector.hpp"

#include "dns.hpp"
#include "message/DNSMessage.hpp"
#include "utils/etsqueue.hpp"
#include "utils/log.hpp"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>


void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

void networkThread(int port,
    utils::ETSQueue<dnslib::DNSMessageL>& inputQueue,
    utils::ETSQueue<dnslib::DNSMessageL>& outputQueue
) {

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    setNonBlocking(sockfd);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    auto _bind = bind(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (_bind < 0) {
        LOG_ERR("Bind failed!");
        return;
    }

    // Regiser epoll for input on udp
    auto epollfd = epoll_create1(0);
    struct epoll_event ev, events[10];
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    auto _epoll_ctl = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
    if (_epoll_ctl == -1) {
        LOG_ERR("Epoll ctl failed!");
        return;
    }

    // Register epoll for output ready in queue
    auto qFd = outputQueue.getEventFd();
    ev.events = EPOLLIN;
    ev.data.fd = qFd;
    _epoll_ctl = epoll_ctl(epollfd, EPOLL_CTL_ADD, qFd, &ev);
    if (_epoll_ctl == -1) {
        LOG_ERR("Epoll ctl failed!");
        return;
    }

    LOG_INFO("Started UDP on socket " + std::to_string(sockfd) + " port " + std::to_string(port));

    while (true) {
        // timeout
        int nfds = epoll_wait(epollfd, events, 10, -1);

        for(int i=0; i<nfds; i++) {
            if (events[i].data.fd == sockfd) {

                std::vector<uint8_t> buffer(512);
                sockaddr_in clientAddr{};
                socklen_t clientAddrLen = sizeof(clientAddr);

                auto _recvfrom = recvfrom(
                    sockfd,
                    buffer.data(), 
                    buffer.size(), 
                    0, 
                    (sockaddr*)&clientAddr,
                    &clientAddrLen
                );

                if (_recvfrom > 0) {
                    buffer.resize(_recvfrom);

                    inputQueue.push({
                        std::move(buffer),
                        clientAddr,
                        sockfd,
                        dnslib::PROTO::UDP
                    });
                }
            }
            else if (events[i].data.fd == qFd) {
                outputQueue.consumeEvent();

                dnslib::DNSMessageL outPacket;
                while (outputQueue.tryPop(outPacket)) {
                    sendto(
                        sockfd, 
                        outPacket.data.data(), 
                        outPacket.data.size(), 
                        0,
                        (sockaddr*)&outPacket.peerAddress,
                        sizeof(outPacket.peerAddress)
                    );
                }

            }
        }
    }
}

void testThread(
    utils::ETSQueue<dnslib::DNSMessageL>& inputQueue,
    utils::ETSQueue<dnslib::DNSMessageL>& outputQueue
) {
    while (true) {
        auto message= inputQueue.pop();

        auto parsed = dnslib::PacketParser::parse(message.data);
        std::cout << parsed.toString() << "\n";;

        outputQueue.push(std::move(message));
    }
}