#pragma once
#include <mutex>
#include <set>
#include <iostream>

class Cache
{
public:
    Cache()
        : m_{}, cache_{}
    {};
    void insert(std::pair<std::string, std::string> req_res)
    {
      std::lock_guard<std::mutex> lock(m_);
      if (cache_.size() < 3000)
        cache_.insert(req_res);
    }

    std::mutex m_;  
    std::map<std::string, std::string> cache_;
};

namespace GlobalCache
{
    //Get the Cache for the application.
    inline Cache& getCache(void)
    {
        static Cache cache;
        return cache;
    }
}