/**
 * @file SubscribersManager.h
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

#include "../CSP/ISerializable.h"
#include "../Concurency/GuardRW.h"

namespace common_serialization
{

class SubscriberBase;

class SubscribersManager
{
public:
    static SubscribersManager& GetSubscribersManager();

    Status AddSubscriber(csp::name_hash_t nameHash, bool multicast, SubscriberBase& subscriber);
    void FindSubscribers(csp::name_hash_t nameHash, Vector<SubscriberBase*>& subscribers);

private:
    static SubscribersManager subscribersManager;

    SubscribersManager() {}

    std::unordered_multimap<csp::name_hash_t, SubscriberBase*> m_subscribersList;
    SharedMutex m_subscribersListMutex;
};

inline SubscribersManager& SubscribersManager::GetSubscribersManager()
{
    return subscribersManager;
}

inline Status SubscribersManager::AddSubscriber(csp::name_hash_t nameHash, bool multicast, SubscriberBase& subscriber)
{
    GuardW guard(m_subscribersListMutex);

    if (!multicast && m_subscribersList.contains(nameHash))
        assert(false);

    m_subscribersList.emplace(std::make_pair(nameHash, &subscriber));

    return Status::kNoError;
}

inline void SubscribersManager::FindSubscribers(csp::name_hash_t nameHash, Vector<SubscriberBase*>& subscribers)
{
    GuardR guard(m_subscribersListMutex);

    auto range = m_subscribersList.equal_range(nameHash);
    while (range.first != range.second)
        subscribers.pushBack(range.first->second);
}

} // namespace common_serialization