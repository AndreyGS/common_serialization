/**
 * @file cslib/include/common_serialization/csp/messaging/DataServersKeeper.h
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

#include "common_serialization/csp/ISerializable.h"
#include "common_serialization/Concurency/GuardRW.h"
#include "common_serialization/Containers/Vector.h"

namespace common_serialization::csp::messaging
{

class IDataServerCommon;

/// @brief Keeper of servers that can process InOutData CSP requests (singleton)
/// @details When server that implement IDataServerCommon interface instantiates,
///     pointer on created instance is placed here 
///     and vice versa when instance is destructed its pointer removed from here too.
class DataServersKeeper
{
public:
    /// @brief Get single instance of DataServersKeeper
    /// @return Reference on single instance of DataServersKeeper
    static DataServersKeeper& GetDataServersKeeper();

    /// @brief Adds server to servers database
    /// @param id Input-struct id that server will handling
    /// @param multicast Is it acceptable to have more than one server to handle this Input-struct
    /// @param pInstance Pointer on server instance
    /// @return Status of operation
    Status addServer(const Id& id, bool multicast, IDataServerCommon* pInstance);

    /// @brief Removes server from servers database
    /// @param id Input-struct id that server was handling
    /// @param pInstance Server instance that must be deleted
    void removeServer(const Id& id, IDataServerCommon* pInstance);

    /// @brief Find all servers that subscribed to handle Input-struct with given id
    /// @tparam T Container capable to hold set of servers
    /// @param id Input-struct id related to handlers
    /// @param servers Container that would be filled with target servers
    /// @return Status of operation.
    ///     If no servers were found, Status::kErrorNoSuchHandler is returned.
    template<typename T>
        requires requires (T t) { { t.pushBack(*(new IDataServerCommon*)) }; { t.clear() }; { t.size() }; }
    Status findServers(const Id& id, T& servers);

    /// @brief Find single server that is subsribed to handle Input-struct with given id
    /// @param id Input-struct id related to handler
    /// @param pServer Pointer on target server
    /// @return Status of operation.
    ///     If no server was found, Status::kErrorNoSuchHandler is returned.
    ///     If there is more than one server that handle this id, Status::kErrorMoreEntires is returned.
    Status findServer(const Id& id, IDataServerCommon*& pServer);

private:
    DataServersKeeper() {}
    DataServersKeeper(const DataServersKeeper&) = delete;
    DataServersKeeper( DataServersKeeper&&) noexcept = delete;
    DataServersKeeper& operator=(const DataServersKeeper&) = delete;
    DataServersKeeper& operator=(DataServersKeeper&&) noexcept = delete;

    std::unordered_multimap<Id, IDataServerCommon*> m_serversList;
    SharedMutex m_serversListMutex;
};

inline DataServersKeeper& DataServersKeeper::GetDataServersKeeper()
{
    static DataServersKeeper serversKeeper;
    return serversKeeper;
}

inline Status DataServersKeeper::addServer(const Id& id, bool multicast, IDataServerCommon* pInstance)
{
    WGuard guard(m_serversListMutex);

    if (!multicast && m_serversList.contains(id))
        assert(false);

    m_serversList.emplace(std::make_pair(id, pInstance));

    return Status::kNoError;
}

inline void DataServersKeeper::removeServer(const Id& id, IDataServerCommon* pInstance)
{
    WGuard guard(m_serversListMutex);

    auto range = m_serversList.equal_range(id);
    while (range.first != range.second)
        if (range.first->second == pInstance)
            range.first = m_serversList.erase(range.first);
        else
            ++range.first;
}

template<typename T>
    requires requires (T t) { { t.pushBack(*(new IDataServerCommon*)) }; { t.clear() }; { t.size() }; }
Status DataServersKeeper::findServers(const Id& id, T& servers)
{
    servers.clear();

    RGuard guard(m_serversListMutex);

    auto range = m_serversList.equal_range(id);
    while (range.first != range.second)
    {
        RUN(servers.pushBack(range.first->second));
        ++range.first;
    }

    return servers.size() ? Status::kNoError : Status::kErrorNoSuchHandler;
}

inline Status DataServersKeeper::findServer(const Id& id, IDataServerCommon*& pServer)
{
    RGuard guard(m_serversListMutex);

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

inline DataServersKeeper& GetDataServersKeeper()
{
    return DataServersKeeper::GetDataServersKeeper();
}

} // namespace common_serialization::csp::messaging
