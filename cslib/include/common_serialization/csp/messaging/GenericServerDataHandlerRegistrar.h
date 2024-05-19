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
    Status registerHandler(const Id& id, bool kMulticast, Service* pService, IServerDataHandlerBase& handler) override;
    void unregisterHandler(const Id& id, IServerDataHandlerBase& handler) noexcept override;
    void unregisterService(Service* pService) noexcept override;
    Status aquireHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& handlers) noexcept override;
    Status aquireHandler(const Id& id, IServerDataHandlerBase*& pHandler) noexcept override;
    void releaseHandler(IServerDataHandlerBase* pHandler) noexcept override;

private:
    struct SdhHandle : public IServerDataHandlerBase
    {
        SdhHandle(Service* pService, IServerDataHandlerBase& handler)
            : pService(pService), handler(handler)
        { }

        SdhHandle(SdhHandle&& rhs) noexcept
            : pService(rhs.pService), handler(rhs.handler)
            , inUseCounter(rhs.inUseCounter.load(std::memory_order_relaxed)), notAvailable(rhs.notAvailable)
        { }

        Status handleDataCommon(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) override
        {
            return handler.handleDataCommon(ctx, clientId, binOutput);
        }

        Service* const pService;
        IServerDataHandlerBase& handler;
        AtomicUint32T inUseCounter{ 0 };
        bool notAvailable{ false };
    };

    struct ToUnregisterHandlerCountdown
    {
        ToUnregisterHandlerCountdown(typename HashMultiMapT<Id, SdhHandle>::iterator it)
            : sem(0), it(it)
        { }
       
        mutable BinarySemaphoreT sem{ 0 };
        typename HashMultiMapT<Id, SdhHandle>::iterator it;
    };

    struct ToUnregisterServiceCountdown
    {
        ToUnregisterServiceCountdown(Service* pService, uint32_t inUseCounter)
            : sem(0), pService(pService), inUseCounter(inUseCounter)
        { }

        mutable BinarySemaphoreT sem{ 0 };
        Service* const pService;
        AtomicUint32T inUseCounter{ 0 };
    };

    void unregisterServiceUnsafe(Service* pService) noexcept;

    HashMultiMapT<Id, SdhHandle> m_handlerMap;
    ListT<ToUnregisterServiceCountdown> m_unregisterServiceCountdownList;
    ListT<ToUnregisterHandlerCountdown> m_unregisterHandlerCountdownList;
    mutable SharedMutex m_handlerMapMutex;
};

inline Status GenericServerDataHandlerRegistrar::registerHandler(const Id& id, bool kMulticast, Service* pService, IServerDataHandlerBase& handler)
{
    WGuard guard(m_handlerMapMutex);

    if (!kMulticast && m_handlerMap.contains(id))
    {
        assert(false);
        return Status::kErrorInvalidArgument;
    }

    m_handlerMap.emplace(std::make_pair(id, SdhHandle{ pService, handler }));

    return Status::kNoError;
}

inline void GenericServerDataHandlerRegistrar::unregisterHandler(const Id& id, IServerDataHandlerBase& handler) noexcept
{
    WGuard guard(m_handlerMapMutex);
    
    auto range = m_handlerMap.equal_range(id);
    while (range.first != range.second)
        if (&range.first->second.handler == &handler)
        {
            if (range.first->second.inUseCounter)
            {
                range.first->second.notAvailable = true;
                m_unregisterHandlerCountdownList.emplace_front(range.first);
                auto it = m_unregisterHandlerCountdownList.begin();
                guard.unlock();
                it->sem.acquire();
                guard.lock();
                m_unregisterHandlerCountdownList.erase(it);
            }
            else
                m_handlerMap.erase(range.first);

            break;
        }
        else
            ++range.first;
}

inline void GenericServerDataHandlerRegistrar::unregisterService(Service* pService) noexcept
{
    WGuard guard(m_handlerMapMutex);

    uint32_t totalInUseCounter = 0;

    for (auto& pair : m_handlerMap)
        if (pair.second.pService == pService)
        {
            pair.second.notAvailable = true;
            totalInUseCounter += pair.second.inUseCounter.load(std::memory_order_relaxed);
        }

    if (totalInUseCounter)
    {
        m_unregisterServiceCountdownList.emplace_front(pService, totalInUseCounter);
        auto it = m_unregisterServiceCountdownList.begin();
        guard.unlock();
        it->sem.acquire();
        guard.lock();
        m_unregisterServiceCountdownList.erase(it);
    }
    else
        unregisterServiceUnsafe(pService);
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

        status = handles.pushBack(&range.first->second);
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

    pHandle = &firstIt->second;
    reinterpret_cast<SdhHandle*>(pHandle)->inUseCounter.fetch_add(1, std::memory_order_relaxed);

    return Status::kNoError;
}

inline void GenericServerDataHandlerRegistrar::releaseHandler(IServerDataHandlerBase* pHandle) noexcept
{
    assert(pHandle);

    RGuard guard(m_handlerMapMutex);

    SdhHandle& handle = *reinterpret_cast<SdhHandle*>(pHandle);
    uint32_t prevUseCount = handle.inUseCounter.fetch_sub(1, std::memory_order_relaxed);

    if (handle.notAvailable)
    {
        bool found{ false };

        Service* pService{ handle.pService };
        for (auto& unregisterServiceCountdown : m_unregisterServiceCountdownList)
            if (unregisterServiceCountdown.pService == pService)
            {
                if (unregisterServiceCountdown.inUseCounter.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    guard.unlock();
                    WGuard wguard(m_handlerMapMutex);
                    unregisterServiceUnsafe(pService);
                    wguard.unlock();
                    unregisterServiceCountdown.sem.release();
                }

                found = true;
                break;
            }

        if (!found && prevUseCount == 1)
            for (auto& unregisterHandlerCountdown : m_unregisterHandlerCountdownList)
                if (&unregisterHandlerCountdown.it->second.handler == &handle.handler)
                {
                    guard.unlock();
                    WGuard wguard(m_handlerMapMutex);
                    m_handlerMap.erase(unregisterHandlerCountdown.it);
                    wguard.unlock();
                    unregisterHandlerCountdown.sem.release();
                    break;
                }
    }
}

inline void GenericServerDataHandlerRegistrar::unregisterServiceUnsafe(Service* pService) noexcept
{
    for (auto it = m_handlerMap.begin(), itEnd = m_handlerMap.end(); it != itEnd;)
        if (it->second.pService == pService)
            it = m_handlerMap.erase(it);
        else
            ++it;
}

} // namespace common_serialization::csp::messaging
