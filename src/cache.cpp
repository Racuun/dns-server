#include "cache.hpp"
#include "utils/log.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <mutex>


//std::optional<std::shared_ptr<std::vector<dnslib::ResourceRecord>>> TLRUCache::get(cacheKey key) {
std::optional<std::shared_ptr<std::vector<std::shared_ptr<dnslib::ResourceRecord>>>> TLRUCache::get(cacheKey key) {
    auto it = cacheMap.find(key);
    if (it == cacheMap.end()) {
        return std::nullopt;
    }

    // Check if expired
    if (std::chrono::steady_clock::now() > it->second->expireTime) {
        list.erase(it->second);
        cacheMap.erase(it);
        LOG_DEBUG("Cache entry expired: " + std::to_string(key.type) + ":"+ key.name);
        return std::nullopt;
    }

    // Move the accessed element to the front of the list (most recently used)
    list.splice(list.begin(), list, it->second);

    // Return the value
    return it->second->value;
}

//void TLRUCache::put(cacheKey key, std::shared_ptr<std::vector<dnslib::ResourceRecord>> value, uint32_t TTL) {
void TLRUCache::put(cacheKey key, std::shared_ptr<std::vector<std::shared_ptr<dnslib::ResourceRecord>>> value, uint32_t TTL) {
    auto it = cacheMap.find(key);
    if (it != cacheMap.end()) {
        list.erase(it->second);
        cacheMap.erase(it);
    }

    auto now = std::chrono::steady_clock::now();
    auto expireTime = now + std::chrono::seconds(TTL);

    list.push_front(CacheEntry{key, value, expireTime});
    cacheMap[key] = list.begin();

    if (cacheMap.size() > static_cast<size_t>(capacity) /*cacheMap.size() > capacity*/) {
        auto keyToEvict = list.back().key;
        list.pop_back();
        cacheMap.erase(keyToEvict);
    }
}