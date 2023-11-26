/**
 * @file cslib/include/common_serialization/CSP/MessagingDataServersKeeper.h
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
#include "common_serialization/Containers/Vector.h"

namespace common_serialization::csp::messaging
{

class IDataServer;

class DataServersKeeper
{
public:
    static DataServersKeeper& GetDataServersKeeper();

    Status addServer(csp::name_hash_t nameHash, bool multicast, IDataServer* pInstance);
    void removeServer(csp::name_hash_t nameHash, IDataServer* pInstance);

    template<typename T>
        requires requires (T t) { { t.pushBack(*(new IDataServer*)) }; { t.clear() }; { t.size() }; }
    Status findServers(csp::name_hash_t nameHash, T& servers);

    inline Status findServer(csp::name_hash_t nameHash, IDataServer*& pServer);

private:
    static DataServersKeeper serversKeeper;

    DataServersKeeper() {}

    std::unordered_multimap<name_hash_t, IDataServer*> m_serversList;
    SharedMutex m_serversListMutex;
};

inline DataServersKeeper DataServersKeeper::serversKeeper;

inline DataServersKeeper& DataServersKeeper::GetDataServersKeeper()
{
    return serversKeeper;
}

inline Status DataServersKeeper::addServer(name_hash_t nameHash, bool multicast, IDataServer* pInstance)
{
    GuardW guard(m_serversListMutex);

    if (!multicast && m_serversList.contains(nameHash))
        assert(false);

    m_serversList.emplace(std::make_pair(nameHash, pInstance));

    return Status::kNoError;
}

inline void DataServersKeeper::removeServer(name_hash_t nameHash, IDataServer* pInstance)
{
    GuardW guard(m_serversListMutex);

    auto range = m_serversList.equal_range(nameHash);
    while (range.first != range.second)
        if (range.first->second == pInstance)
        {
            m_serversList.erase(range.first);
            break;
        }
}

template<typename T>
    requires requires (T t) { { t.pushBack(*(new IDataServer*)) }; { t.clear() }; { t.size() }; }
Status DataServersKeeper::findServers(name_hash_t nameHash, T& servers)
{
    servers.clear();

    GuardR guard(m_serversListMutex);

    auto range = m_serversList.equal_range(nameHash);
    while (range.first != range.second)
    {
        RUN(servers.pushBack(range.first->second));
        ++range.first;
    }

    return servers.size() ? Status::kNoError : Status::kErrorNoSuchHandler;
}

inline Status DataServersKeeper::findServer(csp::name_hash_t nameHash, IDataServer*& pServer)
{
    GuardR guard(m_serversListMutex);

    auto range = m_serversList.equal_range(nameHash);
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

inline DataServersKeeper& GetDataServersKeeper()
{
    return DataServersKeeper::GetDataServersKeeper();
}

} // namespace common_serialization::csp::messaging
