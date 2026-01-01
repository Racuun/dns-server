#include "utils/etsqueue.hpp"
#include <sys/eventfd.h>
#include <unistd.h>
#include <stdexcept>
#include "dns.hpp"

namespace utils {

template <typename T>
ETSQueue<T>::ETSQueue() {
    m_eventFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (m_eventFd == -1) {
        throw std::runtime_error("Failed to create eventfd in Queue");
    }
}

template <typename T>
ETSQueue<T>::~ETSQueue() {
    if (m_eventFd != -1) {
        close(m_eventFd);
    }
}

template <typename T>
void ETSQueue<T>::push(T item) {
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(std::move(item));
    }
    m_cond.notify_one();

    uint64_t u = 1;
    ssize_t ret = write(m_eventFd, &u, sizeof(uint64_t));
    (void)ret;
}

template <typename T>
T ETSQueue<T>::pop() {
    std::unique_lock<std::mutex> lock(m_mutex);

    m_cond.wait(lock, [this]() { return !m_queue.empty(); });

    T item = std::move(m_queue.front());
    m_queue.pop();

    return item;
}

template <typename T>
bool ETSQueue<T>::tryPop(T& outItem) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queue.empty()) {
        return false;
    }
    outItem = std::move(m_queue.front());
    m_queue.pop();
    return true;
}

template <typename T>
bool ETSQueue<T>::empty() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
}

template <typename T>
size_t ETSQueue<T>::size() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size();
}

template <typename T>
int ETSQueue<T>::getEventFd() {
    return m_eventFd;
}

template <typename T>
void ETSQueue<T>::consumeEvent() {
    uint64_t u;
    ssize_t ret = read(m_eventFd, &u, sizeof(uint64_t));
    (void)ret;
}

// Explicit template instantiation
template class ETSQueue<dnslib::DNSMessageL>;

}
