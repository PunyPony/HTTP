#pragma once
#include <mutex>
#include <set>
#include <iostream>

class Cache
{
    Cache();
    void insert(std::string request)
    {
      std::lock_guard<std::mutex> lock(m_);
      if (cache_.size() < 3000)
        cache_.insert(request);
    }

    std::mutex m_;  
    std::set<std::string> cache_;
};