#pragma once

#include <chrono>
#include <cstdint>
#include <list>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>
#include "dns.hpp"

struct cacheKey {
    std::string name;
    dnslib::TYPE type;

    bool operator==(const cacheKey& other) const {
        return name == other.name && type == other.type;
    }
};

namespace std {
template <>
struct hash<cacheKey> {
    size_t operator()(const cacheKey& k) const {
        return hash<string>()(k.name) ^ (hash<uint16_t>()(static_cast<uint16_t>(k.type)) << 1);
    }
};
}


struct CacheEntry {
    cacheKey key;
    std::shared_ptr<std::vector<dnslib::ResourceRecord>> value;
    std::chrono::steady_clock::time_point expireTime;
};


/**
 * @brief Cache for dns records
 * 
 * key - 
 * value - std::shared_ptr<ResourceRecord>
 */
class TLRUCache {

private:
    int capacity;

    using List = std::list<CacheEntry>;

    List list;

    std::unordered_map<cacheKey, List::iterator> cacheMap;

public:
    TLRUCache(int capacity) : capacity(capacity) {}

    std::optional<std::shared_ptr<std::vector<dnslib::ResourceRecord>>> get(cacheKey key);

    void put(cacheKey key, std::shared_ptr<std::vector<dnslib::ResourceRecord>> value, uint32_t TTL);
};