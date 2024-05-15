/**
 * @file cslib/include/common_serialization/csp/messaging/GenericServerDataHandlerRegistrar.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023-2024 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

#include "common_serialization/csp/messaging/IServerDataHandlerRegistrar.h"

namespace common_serialization::csp::messaging
{

class GenericServerDataHandlerRegistrar : public IServerDataHandlerRegistrar
{
public:
    Status registerHandler(const Id& id, bool kMulticast, IServerDataHandlerBase* pInstance) override;
    void unregisterHandler(const Id& id, IServerDataHandlerBase* pInstance) noexcept override;
    Status aquireHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& instances) noexcept override;
    Status aquireHandler(const Id& id, IServerDataHandlerBase*& pInstance) noexcept override;
    void releaseHandler(const Id& id, IServerDataHandlerBase* pInstance) noexcept override;

private:
    struct SdhHandle
    {
        explicit SdhHandle(IServerDataHandlerBase* pInstance)
            : pInstance(pInstance)
        { }

        SdhHandle(SdhHandle&& rhs) noexcept
            : pInstance(rhs.pInstance), inUseCounter(rhs.inUseCounter.load(std::memory_order_relaxed)), notAvailable(rhs.notAvailable)
        { }

        IServerDataHandlerBase* pInstance{ nullptr };
        AtomicUint32T inUseCounter{ 0 };
        bool notAvailable{ false };
    };

    struct ToUnregisterCountdown
    {
        ToUnregisterCountdown(IServerDataHandlerBase* pInstance, uint32_t totalInUseCounter)
            : sem(0), pInstance(pInstance), totalInUseCounter(totalInUseCounter)
        { }

        mutable BinarySemaphoreT sem{ 0 };
        IServerDataHandlerBase* pInstance{ nullptr };
        AtomicUint32T totalInUseCounter{ 0 };
    };

    void unregisterInstanceUnsafe(IServerDataHandlerBase* pInstance) noexcept;

    HashMultiMapT<Id, SdhHandle> m_serverList;
    ListT<ToUnregisterCountdown> m_unregisterCountdownList;
    mutable SharedMutex m_serverListMutex;
};

inline Status GenericServerDataHandlerRegistrar::registerHandler(const Id& id, bool kMulticast, IServerDataHandlerBase* pInstance)
{
    WGuard guard(m_serverListMutex);

    if (!kMulticast && m_serverList.contains(id))
        assert(false);

    m_serverList.emplace(std::make_pair(id, SdhHandle{ pInstance }));

    return Status::kNoError;
}

inline void GenericServerDataHandlerRegistrar::unregisterHandler(const Id& id, IServerDataHandlerBase* pInstance) noexcept
{
    {
        bool notRemovedYet{ false };
        RGuard guard(m_serverListMutex);
        auto range = m_serverList.equal_range(id);
        while (range.first != range.second)
            if (range.first->second.pInstance == pInstance)
            {
                notRemovedYet = true;
                break;
            }
            else
                ++range.first;

        if (!notRemovedYet)
            return;
    }

    WGuard guard(m_serverListMutex);

    uint32_t totalInUseCounter = 0;

    for (auto& pair : m_serverList)
        if (pair.second.pInstance == pInstance)
        {
            pair.second.notAvailable = true;
            totalInUseCounter += pair.second.inUseCounter.load(std::memory_order_relaxed);
        }
    
    if (totalInUseCounter)
    {
        m_unregisterCountdownList.emplace_front(pInstance, totalInUseCounter);
        auto it = m_unregisterCountdownList.begin();
        guard.unlock();
        it->sem.acquire();
        guard.lock();
        m_unregisterCountdownList.erase(it);
    }
    else
        unregisterInstanceUnsafe(pInstance);
}

inline Status GenericServerDataHandlerRegistrar::aquireHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& instances) noexcept
{
    Status status{ Status::kNoError };
    bool wasNotAvailable{ false };

    instances.clear();

    RGuard guard(m_serverListMutex);

    auto range = m_serverList.equal_range(id);
    auto rangeFirstCopy = range.first;

    while (range.first != range.second)
    {
        if (range.first->second.notAvailable)
        {
            wasNotAvailable = true;
            continue;
        }

        status = instances.pushBack(range.first->second.pInstance);
        if (!statusSuccess(status))
        {
            while (rangeFirstCopy != range.first)
                rangeFirstCopy->second.inUseCounter.fetch_sub(1, std::memory_order_relaxed);;

            return status;
        }

        range.first->second.inUseCounter.fetch_add(1, std::memory_order_relaxed);
        ++range.first;
    }

    return instances.size() ? Status::kNoError : wasNotAvailable ? Status::kErrorNotAvailible : Status::kErrorNoSuchHandler;
}

inline Status GenericServerDataHandlerRegistrar::aquireHandler(const Id& id, IServerDataHandlerBase*& pInstance) noexcept
{
    RGuard guard(m_serverListMutex);

    auto range = m_serverList.equal_range(id);

    if (range.first == range.second)
        return Status::kErrorNoSuchHandler;

    auto firstIt = range.first++;

    if (range.first != range.second)
        return Status::kErrorMoreEntires;

    if (firstIt->second.notAvailable)
        return Status::kErrorNotAvailible;

    pInstance = firstIt->second.pInstance;
    firstIt->second.inUseCounter.fetch_add(1, std::memory_order_relaxed);;

    return Status::kNoError;
}

inline void GenericServerDataHandlerRegistrar::releaseHandler(const Id& id, IServerDataHandlerBase* pInstance) noexcept
{
    RGuard guard(m_serverListMutex);

    auto range = m_serverList.equal_range(id);

    if (range.first == range.second)
        return;

    while (range.first != range.second)
        if (range.first->second.pInstance == pInstance)
        {
            range.first->second.inUseCounter.fetch_sub(1, std::memory_order_relaxed);
            break;
        }
        else
            ++range.first;

    if (range.first->second.notAvailable)
        for (auto it = m_unregisterCountdownList.begin(), itEnd = m_unregisterCountdownList.end(); it != itEnd; ++it)
            if (it->pInstance == pInstance)
            {
                if (it->totalInUseCounter.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    guard.unlock();
                    WGuard wguard(m_serverListMutex);
                    unregisterInstanceUnsafe(pInstance);
                    it->sem.release();
                }

                break;
            }
}

inline void GenericServerDataHandlerRegistrar::unregisterInstanceUnsafe(IServerDataHandlerBase* pInstance) noexcept
{
    for (auto it = m_serverList.begin(), itEnd = m_serverList.end(); it != itEnd;)
        if (it->second.pInstance == pInstance)
            it = m_serverList.erase(it);
        else
            ++it;
}

} // namespace common_serialization::csp::messaging
