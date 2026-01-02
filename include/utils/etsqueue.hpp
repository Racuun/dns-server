#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>


namespace utils {

template<typename T>
class ETSQueue {
private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cond;

    int m_eventFd;
public:
    ETSQueue();
    ~ETSQueue();

    void push(T item);
    T pop();
    bool tryPop(T& outItem);

    bool empty() const;
    size_t size() const;

    int getEventFd();
    void consumeEvent();
};

}