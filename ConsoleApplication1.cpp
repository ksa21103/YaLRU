// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <map>
#include <list>
#include <string.h>

class LRU
{
    struct Value;

    using Dict = std::map<std::string, Value>;
    using Queue = std::list<Dict::iterator>;

    struct Value
    {
        std::string data;
        Queue::iterator mapIt;

        Value(std::string&& value)
            : data(std::move(value))
        {}
    };

    const size_t m_maxItems;
    Dict         m_dict;
    Queue        m_queue;

public:
    LRU(size_t maxItems)
        : m_maxItems(maxItems)
    {}

    void put(std::string key, std::string value)
    {
        if (std::string *p = get(key))
        {
            *p = std::move(value);
        }
        else
        {
            if (m_dict.size() == m_maxItems)
            {
                m_dict.erase(m_queue.front());
                m_queue.pop_front();
            }
            auto it = m_dict.emplace(std::move(key), std::move(value)).first;
            it->second.mapIt = m_queue.insert(m_queue.end(), it);
        }
    }

    std::string *get(const std::string& key)
    {
        auto it = m_dict.find(key);
        if (it != m_dict.end())
        {
            m_queue.splice(m_queue.end(), m_queue, it->second.mapIt);
            return &it->second.data;
        }
        else
        {
            return nullptr;
        }
    }
};

int main()
{
    LRU lru(2);
    lru.put("A", "1");
    lru.put("B", "2");
    //lru.get("A") != nullptr;
    lru.put("C", "3");
    //assert(lru.get("B") == nullptr);

    return 0;
}
