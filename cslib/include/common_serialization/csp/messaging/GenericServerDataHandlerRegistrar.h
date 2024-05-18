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
    Status registerHandler(const Id& id, bool kMulticast, void* pServiceInstance, IServerDataHandlerBase* pHandler) override;
    void unregisterHandler(const Id& id, IServerDataHandlerBase* pHandler) noexcept override;
    Status aquireHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& handlers) noexcept override;
    Status aquireHandler(const Id& id, IServerDataHandlerBase*& pHandler) noexcept override;
    void releaseHandler(const Id& id, IServerDataHandlerBase* pHandler) noexcept override;

private:
    struct SdhHandle : public IServerDataHandlerBase
    {
        SdhHandle(void* pServiceInstance, IServerDataHandlerBase* pHandler)
            : pServiceInstance(pServiceInstance), pHandler(pHandler)
        { }

        SdhHandle(SdhHandle&& rhs) noexcept
            : pServiceInstance(rhs.pServiceInstance), pHandler(rhs.pHandler)
            , inUseCounter(rhs.inUseCounter.load(std::memory_order_relaxed)), notAvailable(rhs.notAvailable)
        { }

        Status handleDataCommon(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) override
        {
            return pHandler->handleDataCommon(ctx, clientId, binOutput);
        }

        void* pServiceInstance{ nullptr };
        IServerDataHandlerBase* pHandler{ nullptr };
        AtomicUint32T inUseCounter{ 0 };
        bool notAvailable{ false };
    };

    struct ToUnregisterCountdown
    {
        ToUnregisterCountdown(void* pServiceInstance, uint32_t totalInUseCounter)
            : sem(0), pServiceInstance(pServiceInstance), totalInUseCounter(totalInUseCounter)
        { }

        mutable BinarySemaphoreT sem{ 0 };
        void* pServiceInstance{ nullptr };
        AtomicUint32T totalInUseCounter{ 0 };
    };

    void unregisterInstanceUnsafe(void* pServiceInstance) noexcept;

    HashMultiMapT<Id, SdhHandle> m_handlerMap;
    HashMultiMapT<Id, SdhHandle> m_tempHandlerMap;
    ListT<ToUnregisterCountdown> m_unregisterCountdownList;
    mutable SharedMutex m_handlerMapMutex;
};

inline Status GenericServerDataHandlerRegistrar::registerHandler(const Id& id, bool kMulticast, void* pServiceInstance, IServerDataHandlerBase* pHandler)
{
    WGuard guard(m_handlerMapMutex);

    if (!kMulticast && m_handlerMap.contains(id))
        assert(false);

    m_handlerMap.emplace(std::make_pair(id, SdhHandle{ pServiceInstance, pHandler }));

    return Status::kNoError;
}

inline void GenericServerDataHandlerRegistrar::unregisterHandler(const Id& id, IServerDataHandlerBase* pHandler) noexcept
{
    void* pServiceInstance{ nullptr };

    {
        bool notRemovedYet{ false };
        RGuard guard(m_handlerMapMutex);
        auto range = m_handlerMap.equal_range(id);
        while (range.first != range.second)
            if (range.first->second.pHandler == pHandler)
            {
                notRemovedYet = true;
                pServiceInstance = range.first->second.pServiceInstance;
                break;
            }
            else
                ++range.first;

        if (!notRemovedYet)
            return;
    }

    WGuard guard(m_handlerMapMutex);

    uint32_t totalInUseCounter = 0;

    for (auto& pair : m_handlerMap)
        if (pair.second.pServiceInstance == pServiceInstance)
        {
            pair.second.notAvailable = true;
            totalInUseCounter += pair.second.inUseCounter.load(std::memory_order_relaxed);
        }
    
    if (totalInUseCounter)
    {
        m_unregisterCountdownList.emplace_front(pServiceInstance, totalInUseCounter);
        auto it = m_unregisterCountdownList.begin();
        guard.unlock();
        it->sem.acquire();
        guard.lock();
        m_unregisterCountdownList.erase(it);
    }
    else
        unregisterInstanceUnsafe(pServiceInstance);
}

inline Status GenericServerDataHandlerRegistrar::aquireHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& handles) noexcept
{
    Status status{ Status::kNoError };
    bool wasNotAvailable{ false };

    handles.clear();

    RGuard guard(m_handlerMapMutex);

    for (auto range = m_handlerMap.equal_range(id); range.first != range.second; ++range.first)
    {
        if (range.first->second.notAvailable)
        {
            wasNotAvailable = true;
            continue;
        }

        //status = handles.pushBack(&range.first->second);
        status = handles.pushBack(range.first->second.pHandler);
        if (!statusSuccess(status))
        {
            for (auto& handle : handles)
                reinterpret_cast<SdhHandle*>(handle)->inUseCounter.fetch_sub(1, std::memory_order_relaxed);;

            handles.clear();

            return status;
        }

        range.first->second.inUseCounter.fetch_add(1, std::memory_order_relaxed);
    }

    return handles.size() ? Status::kNoError : wasNotAvailable ? Status::kErrorNotAvailible : Status::kErrorNoSuchHandler;
}

inline Status GenericServerDataHandlerRegistrar::aquireHandler(const Id& id, IServerDataHandlerBase*& pHandle) noexcept
{
    RGuard guard(m_handlerMapMutex);

    auto range = m_handlerMap.equal_range(id);

    if (range.first == range.second)
        return Status::kErrorNoSuchHandler;

    auto firstIt = range.first++;

    if (range.first != range.second)
        return Status::kErrorMoreEntires;

    if (firstIt->second.notAvailable)
        return Status::kErrorNotAvailible;

   // pHandle = &firstIt->second;
    //reinterpret_cast<SdhHandle*>(pHandle)->inUseCounter.fetch_add(1, std::memory_order_relaxed);
    pHandle = firstIt->second.pHandler;
    firstIt->second.inUseCounter.fetch_add(1, std::memory_order_relaxed);

    return Status::kNoError;
}

inline void GenericServerDataHandlerRegistrar::releaseHandler(const Id& id, IServerDataHandlerBase* pHandle) noexcept
{
    RGuard guard(m_handlerMapMutex);

    //SdhHandle& handle = *reinterpret_cast<SdhHandle*>(pHandle);

    //handle.inUseCounter.fetch_sub(1, std::memory_order_relaxed);

    auto range = m_handlerMap.equal_range(id);

    while (range.first != range.second)
        if (range.first->second.pHandler == pHandle)
        {
            range.first->second.inUseCounter.fetch_sub(1, std::memory_order_relaxed);
            break;
        }
        else
            ++range.first;



    //if (handle.notAvailable)
    if (range.first->second.notAvailable)
    {
        void* pServiceInstance{ range.first->second.pServiceInstance };
        for (auto& unregisterCountdown : m_unregisterCountdownList)
            if (unregisterCountdown.pServiceInstance == pServiceInstance)
            {
                if (unregisterCountdown.totalInUseCounter.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    guard.unlock();
                    WGuard wguard(m_handlerMapMutex);
                    unregisterInstanceUnsafe(pServiceInstance);
                    wguard.unlock();
                    unregisterCountdown.sem.release();
                }

                break;
            }
    }
}

inline void GenericServerDataHandlerRegistrar::unregisterInstanceUnsafe(void* pServiceInstance) noexcept
{
    for (auto it = m_handlerMap.begin(), itEnd = m_handlerMap.end(); it != itEnd;)
        if (it->second.pServiceInstance == pServiceInstance)
            it = m_handlerMap.erase(it);
        else
            ++it;
}

} // namespace common_serialization::csp::messaging
