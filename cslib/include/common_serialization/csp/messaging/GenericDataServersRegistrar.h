/**
 * @file cslib/include/common_serialization/csp/messaging/GenericDataServersRegistrar.h
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

#include "common_serialization/csp/messaging/IDataHandlersRegistrar.h"

namespace common_serialization::csp::messaging
{

template<SdContainers T = traits::DefaultSdContainers>
class GenericDataServersRegistrar : public IDataHandlersRegistrar<T>
{
public:
    Status addHandler(const Id& id, bool kMulticast, IDataHandlerBase<T>* pInstance) override;
    void removeHandler(const Id& id, IDataHandlerBase<T>* pInstance) noexcept override;
    Status findHandlers(const Id& id, Vector<IDataHandlerBase<T>*, RawStrategicAllocatorHelper<IDataHandlerBase<T>*>>& servers) const noexcept override;
    Status findHandler(const Id& id, IDataHandlerBase<T>*& pServer) const noexcept override;

private:
    std::unordered_multimap<Id, IDataHandlerBase<T>*> m_serversList;
    mutable SharedMutex m_serverListMutex;
};

template<SdContainers T>
Status GenericDataServersRegistrar<T>::addHandler(const Id& id, bool kMulticast, IDataHandlerBase<T>* pInstance)
{
    WGuard guard(m_serverListMutex);

    if (!kMulticast && m_serversList.contains(id))
        assert(false);

    m_serversList.emplace(std::make_pair(id, pInstance));

    return Status::kNoError;
}

template<SdContainers T>
void GenericDataServersRegistrar<T>::removeHandler(const Id& id, IDataHandlerBase<T>* pInstance) noexcept
{
    WGuard guard(m_serverListMutex);

    auto range = m_serversList.equal_range(id);
    while (range.first != range.second)
        if (range.first->second == pInstance)
            range.first = m_serversList.erase(range.first);
        else
            ++range.first;
}

template<SdContainers T>
Status GenericDataServersRegistrar<T>::findHandlers(const Id& id, Vector<IDataHandlerBase<T>*, RawStrategicAllocatorHelper<IDataHandlerBase<T>*>>& servers) const noexcept
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

template<SdContainers T>
Status GenericDataServersRegistrar<T>::findHandler(const Id& id, IDataHandlerBase<T>*& pServer) const noexcept
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
