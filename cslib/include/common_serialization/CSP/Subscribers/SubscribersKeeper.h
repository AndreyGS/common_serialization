/**
 * @file SubscribersKeeper.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include "common_serialization/CSP/ISerializable.h"
#include "common_serialization/Concurency/GuardRW.h"

namespace common_serialization
{

namespace csp
{

class SubscriberBase;

struct SubscriberDescriptor
{
    using SubscriberHandleCommon = Status(*)(void*, BinWalker&, BinVector&);

    void* instance{ nullptr };
    SubscriberHandleCommon handleCommon{ nullptr };
    
    SubscriberDescriptor() { }

    SubscriberDescriptor(void* instanceIn, SubscriberHandleCommon handleCommonIn)
        : instance(instanceIn), handleCommon(handleCommonIn)
    { }
};

class SubscribersKeeper
{
public:
    static SubscribersKeeper& GetSubscribersManager();

    Status addSubscriber(csp::name_hash_t nameHash, bool multicast, SubscriberBase* pInstance);
    void removeSubscriber(csp::name_hash_t nameHash, SubscriberBase* pInstance);
    void findSubscribers(csp::name_hash_t nameHash, Vector<SubscriberBase*, RawGenericAllocatorHelper<SubscriberBase*>>& subscribers);

private:
    static SubscribersKeeper subscribersManager;

    SubscribersKeeper() {}

    std::unordered_multimap<name_hash_t, SubscriberBase*> m_subscribersList;
    SharedMutex m_subscribersListMutex;
};

inline SubscribersKeeper SubscribersKeeper::subscribersManager;

inline SubscribersKeeper& SubscribersKeeper::GetSubscribersManager()
{
    return subscribersManager;
}

inline Status SubscribersKeeper::addSubscriber(name_hash_t nameHash, bool multicast, SubscriberBase* pInstance)
{
    GuardW guard(m_subscribersListMutex);

    if (!multicast && m_subscribersList.contains(nameHash))
        assert(false);

    m_subscribersList.emplace(std::make_pair(nameHash, pInstance));

    return Status::kNoError;
}

inline void SubscribersKeeper::removeSubscriber(name_hash_t nameHash, SubscriberBase* pInstance)
{
    GuardW guard(m_subscribersListMutex);

    auto range = m_subscribersList.equal_range(nameHash);
    while (range.first != range.second)
        if (range.first->second == pInstance)
        {
            m_subscribersList.erase(range.first);
            break;
        }
}

inline void SubscribersKeeper::findSubscribers(name_hash_t nameHash, Vector<SubscriberBase*, RawGenericAllocatorHelper<SubscriberBase*>>& subscribers)
{
    subscribers.clear();

    GuardR guard(m_subscribersListMutex);

    auto range = m_subscribersList.equal_range(nameHash);
    while (range.first != range.second)
    {
        subscribers.pushBack(range.first->second);
        ++range.first;
    }
}

inline SubscribersKeeper& GetSubscribersManager()
{
    return SubscribersKeeper::GetSubscribersManager();
}

} // namespace csp

} // namespace common_serialization
