/**
 * @file common_serialization/csp_messaging/GenericServerDataHandlerRegistrar.h
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

#include <common_serialization/concurrency_interfaces/GuardRW.h>
#include <common_serialization/csp_messaging/csp_messaging_config.h>
#include <common_serialization/csp_messaging/IServerDataHandlerRegistrar.h>

namespace common_serialization::csp::messaging
{

class GenericServerDataHandlerRegistrar : public IServerDataHandlerRegistrar
{
public:
    Status registerHandler(const Id& id, bool kMulticast, Service* pService, IServerDataHandlerBase& handler) override;
    void unregisterHandler(const Id& id, IServerDataHandlerBase& handler) noexcept override;
    void unregisterService(Service* pService) noexcept override;
    Status aquireHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& handlers) override;
    Status aquireHandler(const Id& id, IServerDataHandlerBase*& pHandler) noexcept override;
    void releaseHandler(IServerDataHandlerBase* pHandler) noexcept override;

private:
    struct SdhHandle : IServerDataHandlerBase
    {
        SdhHandle(Service* pService, IServerDataHandlerBase& handler)
            : pService(pService), handler(handler)
        { }

        SdhHandle(SdhHandle&& rhs) noexcept
            : pService(rhs.pService), handler(rhs.handler)
            , inUseCounter(rhs.inUseCounter.load(std::memory_order_relaxed)), state(rhs.state)
        { }

        Status handleDataCommon(context::DData& ctx, const GenericPointerKeeperT& clientId, BinVectorT& binOutput) override
        {
            return handler.handleDataCommon(ctx, clientId, binOutput);
        }

        enum class State
        {
            Active,
            HandlerRemove,
            ServiceRemove
        };

        Service* const pService{ nullptr };
        IServerDataHandlerBase& handler;
        AtomicUint32T inUseCounter{ 0 };
        State state{ State::Active };
    };

    struct ToUnregisterHandler
    {
        ToUnregisterHandler(const Id& id, SdhHandle& handle)
            : done(1), id(id), handle(handle)
        { }
       
        mutable LatchT done{ 1 };
        const Id& id;
        SdhHandle& handle;
    };

    struct ToUnregisterService
    {
        ToUnregisterService(Service* pService, uint32_t inUseCounter)
            : done(1), pService(pService), inUseCounter(inUseCounter)
        { }

        mutable LatchT done{ 1 };
        Service* const pService{ nullptr };
        AtomicUint32T inUseCounter{ 0 };
    };

    HashMultiMapT<Id, SdhHandle> m_handlerMap;
    ListT<ToUnregisterService> m_unregisterServiceList;
    ListT<ToUnregisterHandler> m_unregisterHandlerList;
    mutable SharedMutexT m_handlerMapMutex;
};

inline Status GenericServerDataHandlerRegistrar::registerHandler(const Id& id, bool kMulticast, Service* pService, IServerDataHandlerBase& handler)
{
    WGuard guard(m_handlerMapMutex);

    if (!kMulticast && m_handlerMap.contains(id))
    {
        assert(false);
        return Status::ErrorInvalidArgument;
    }

    m_handlerMap.emplace(std::make_pair(id, SdhHandle{ pService, handler }));

    return Status::NoError;
}

inline void GenericServerDataHandlerRegistrar::unregisterHandler(const Id& id, IServerDataHandlerBase& handler) noexcept
{
    WGuard guard(m_handlerMapMutex);
    
    auto range = m_handlerMap.equal_range(id);
    while (range.first != range.second)
        if (&range.first->second.handler == &handler)
        {
            bool deferred = false;

            if (range.first->second.inUseCounter)
            {
                deferred = true;
                range.first->second.state = SdhHandle::State::HandlerRemove;
                m_unregisterHandlerList.emplace_front(range.first->first, range.first->second);
                auto it = m_unregisterHandlerList.begin();
                guard.unlock();
                it->done.wait();
                guard.lock();
                m_unregisterHandlerList.erase(it);
            }
            
            // If we've been waiting of releasing handler there can be possibility of map rehashing.
            // If it is true, C++ standard say that all unordered_map iterators will be invalidated.
            // So we can't just pass to erase function previously found iterator.
            if (deferred)
            {
                auto newRange = m_handlerMap.equal_range(id);
                while (newRange.first != newRange.second)
                    if (&newRange.first->second.handler == &handler)
                    {
                        m_handlerMap.erase(newRange.first);
                        break;
                    }
                    else
                        ++newRange.first;
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
            pair.second.state = SdhHandle::State::ServiceRemove;
            totalInUseCounter += pair.second.inUseCounter.load(std::memory_order_relaxed);
        }

    if (totalInUseCounter)
    {
        m_unregisterServiceList.emplace_front(pService, totalInUseCounter);
        auto it = m_unregisterServiceList.begin();
        guard.unlock();
        it->done.wait();
        guard.lock();
        m_unregisterServiceList.erase(it);
    }

    for (auto it = m_handlerMap.begin(), itEnd = m_handlerMap.end(); it != itEnd;)
        if (it->second.pService == pService)
            it = m_handlerMap.erase(it);
        else
            ++it;
}

inline Status GenericServerDataHandlerRegistrar::aquireHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& handles)
{
    Status status{ Status::NoError };
    bool wasNotAvailable{ false };

    handles.clear();

    RGuard guard(m_handlerMapMutex);

    for (auto range = m_handlerMap.equal_range(id); range.first != range.second; ++range.first)
    {
        if (range.first->second.state != SdhHandle::State::Active)
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

    return handles.size() ? Status::NoError : wasNotAvailable ? Status::ErrorNotAvailible : Status::ErrorNoSuchHandler;
}

inline Status GenericServerDataHandlerRegistrar::aquireHandler(const Id& id, IServerDataHandlerBase*& pHandle) noexcept
{
    RGuard guard(m_handlerMapMutex);

    auto range = m_handlerMap.equal_range(id);

    if (range.first == range.second)
        return Status::ErrorNoSuchHandler;

    auto firstIt = range.first++;

    if (range.first != range.second)
        return Status::ErrorMoreEntires;

    if (firstIt->second.state != SdhHandle::State::Active)
        return Status::ErrorNotAvailible;

    pHandle = &firstIt->second;
    reinterpret_cast<SdhHandle*>(pHandle)->inUseCounter.fetch_add(1, std::memory_order_relaxed);

    return Status::NoError;
}

inline void GenericServerDataHandlerRegistrar::releaseHandler(IServerDataHandlerBase* pHandle) noexcept
{
    assert(pHandle);

    RGuard guard(m_handlerMapMutex);

    SdhHandle& handle = *reinterpret_cast<SdhHandle*>(pHandle);
    uint32_t prevUseCount = handle.inUseCounter.fetch_sub(1, std::memory_order_relaxed);

    switch (handle.state)
    {
    case SdhHandle::State::ServiceRemove:
    {
        for (auto& unregisterService : m_unregisterServiceList)
            if (unregisterService.pService == handle.pService)
            {
                if (unregisterService.inUseCounter.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    guard.unlock();
                    unregisterService.done.count_down();
                }
                break;
            }
    }
    case SdhHandle::State::HandlerRemove:
    {
        if (prevUseCount == 1)
        {
            for (auto& unregisterHandler : m_unregisterHandlerList)
                if (&unregisterHandler.handle.handler == &handle.handler)
                {
                    guard.unlock();
                    unregisterHandler.done.count_down();
                    break;
                }
        }
        break;
    }
    default:
        break;
    }
}

} // namespace common_serialization::csp::messaging
