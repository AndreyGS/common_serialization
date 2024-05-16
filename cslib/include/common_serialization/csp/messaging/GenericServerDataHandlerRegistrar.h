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
    struct SdhHandle : public IServerDataHandlerBase
    {
        explicit SdhHandle(IServerDataHandlerBase* pInstance)
            : pInstance(pInstance)
        { }

        SdhHandle(SdhHandle&& rhs) noexcept
            : pInstance(rhs.pInstance), inUseCounter(rhs.inUseCounter.load(std::memory_order_relaxed)), notAvailable(rhs.notAvailable)
        { }

        Status handleDataCommon(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) override
        {
            return pInstance->handleDataCommon(ctx, clientId, binOutput);
        }

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

    HashMultiMapT<Id, SdhHandle> m_handlerList;
    ListT<ToUnregisterCountdown> m_unregisterCountdownList;
    mutable SharedMutex m_handlerListMutex;
};

inline Status GenericServerDataHandlerRegistrar::registerHandler(const Id& id, bool kMulticast, IServerDataHandlerBase* pInstance)
{
    WGuard guard(m_handlerListMutex);

    if (!kMulticast && m_handlerList.contains(id))
        assert(false);

    m_handlerList.emplace(std::make_pair(id, SdhHandle{ pInstance }));

    return Status::kNoError;
}

inline void GenericServerDataHandlerRegistrar::unregisterHandler(const Id& id, IServerDataHandlerBase* pInstance) noexcept
{
    {
        bool notRemovedYet{ false };
        RGuard guard(m_handlerListMutex);
        auto range = m_handlerList.equal_range(id);
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

    WGuard guard(m_handlerListMutex);

    uint32_t totalInUseCounter = 0;

    for (auto& pair : m_handlerList)
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

    RGuard guard(m_handlerListMutex);

    for (auto range = m_handlerList.equal_range(id); range.first != range.second; ++range.first)
    {
        if (range.first->second.notAvailable)
        {
            wasNotAvailable = true;
            continue;
        }

        status = instances.pushBack(&range.first->second);
        if (!statusSuccess(status))
        {
            for (auto& instance : instances)
                reinterpret_cast<SdhHandle*>(instance)->inUseCounter.fetch_sub(1, std::memory_order_relaxed);;

            instances.clear();

            return status;
        }

        range.first->second.inUseCounter.fetch_add(1, std::memory_order_relaxed);
    }

    return instances.size() ? Status::kNoError : wasNotAvailable ? Status::kErrorNotAvailible : Status::kErrorNoSuchHandler;
}

inline Status GenericServerDataHandlerRegistrar::aquireHandler(const Id& id, IServerDataHandlerBase*& pInstance) noexcept
{
    RGuard guard(m_handlerListMutex);

    auto range = m_handlerList.equal_range(id);

    if (range.first == range.second)
        return Status::kErrorNoSuchHandler;

    auto firstIt = range.first++;

    if (range.first != range.second)
        return Status::kErrorMoreEntires;

    if (firstIt->second.notAvailable)
        return Status::kErrorNotAvailible;

    pInstance = &firstIt->second;
    reinterpret_cast<SdhHandle*>(pInstance)->inUseCounter.fetch_add(1, std::memory_order_relaxed);;

    return Status::kNoError;
}

inline void GenericServerDataHandlerRegistrar::releaseHandler(const Id& id, IServerDataHandlerBase* pInstance) noexcept
{
    RGuard guard(m_handlerListMutex);

    SdhHandle& handle = *reinterpret_cast<SdhHandle*>(pInstance);

    handle.inUseCounter.fetch_sub(1, std::memory_order_relaxed);

    if (handle.notAvailable)
        for (auto& unregisterCountdown : m_unregisterCountdownList)
            if (unregisterCountdown.pInstance == handle.pInstance)
            {
                if (unregisterCountdown.totalInUseCounter.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    guard.unlock();
                    WGuard wguard(m_handlerListMutex);
                    unregisterInstanceUnsafe(handle.pInstance);
                    unregisterCountdown.sem.release();
                }

                break;
            }
}

inline void GenericServerDataHandlerRegistrar::unregisterInstanceUnsafe(IServerDataHandlerBase* pInstance) noexcept
{
    for (auto it = m_handlerList.begin(), itEnd = m_handlerList.end(); it != itEnd;)
        if (it->second.pInstance == pInstance)
            it = m_handlerList.erase(it);
        else
            ++it;
}

} // namespace common_serialization::csp::messaging
