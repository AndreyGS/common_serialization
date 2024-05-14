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
    Status addHandler(const Id& id, bool kMulticast, IServerDataHandlerBase* pInstance) override;
    void removeHandler(const Id& id, IServerDataHandlerBase* pInstance) noexcept override;
    Status findHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& servers) const noexcept override;
    Status findHandler(const Id& id, IServerDataHandlerBase*& pServer) const noexcept override;

private:
    HashMapT<Id, IServerDataHandlerBase*> m_serversList;
    mutable SharedMutex m_serverListMutex;
};

inline Status GenericServerDataHandlerRegistrar::addHandler(const Id& id, bool kMulticast, IServerDataHandlerBase* pInstance)
{
    WGuard guard(m_serverListMutex);

    if (!kMulticast && m_serversList.contains(id))
        assert(false);

    m_serversList.emplace(std::make_pair(id, pInstance));

    return Status::kNoError;
}

inline void GenericServerDataHandlerRegistrar::removeHandler(const Id& id, IServerDataHandlerBase* pInstance) noexcept
{
    WGuard guard(m_serverListMutex);

    auto range = m_serversList.equal_range(id);
    while (range.first != range.second)
        if (range.first->second == pInstance)
        {
            m_serversList.erase(range.first);
            return;
        }
        else
            ++range.first;
}

inline Status GenericServerDataHandlerRegistrar::findHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& servers) const noexcept
{
    servers.clear();

    RGuard guard(m_serverListMutex);

    auto range = m_serversList.equal_range(id);
    while (range.first != range.second)
    {
        CS_RUN(servers.pushBack(range.first->second));
        ++range.first;
    }

    return servers.size() ? Status::kNoError : Status::kErrorNoSuchHandler;
}

inline Status GenericServerDataHandlerRegistrar::findHandler(const Id& id, IServerDataHandlerBase*& pServer) const noexcept
{
    RGuard guard(m_serverListMutex);

    auto range = m_serversList.equal_range(id);
    if (range.first != range.second)
    {
        pServer = range.first->second;

        if (++range.first != range.second)
            return Status::kErrorMoreEntires;
        else
            return Status::kNoError;
    }

    return Status::kErrorNoSuchHandler;
}

} // namespace common_serialization::csp::messaging
