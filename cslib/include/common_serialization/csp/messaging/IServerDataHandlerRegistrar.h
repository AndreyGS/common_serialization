/**
 * @file cslib/include/common_serialization/csp/messaging/IServerDataHandlerRegistrar.h
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

#include <common_serialization/csp/messaging/IServerDataHandlerBase.h>

namespace common_serialization::csp::messaging
{

class IServerDataHandlerBase;

using Service = void;

/// @brief Registrar of servers that can process InOutData CSP requests
class IServerDataHandlerRegistrar
{
public:
    IServerDataHandlerRegistrar() = default;

    IServerDataHandlerRegistrar(const IServerDataHandlerRegistrar&) = delete;
    IServerDataHandlerRegistrar& operator=(const IServerDataHandlerRegistrar&) = delete;

    IServerDataHandlerRegistrar(IServerDataHandlerRegistrar&&) noexcept = default;
    IServerDataHandlerRegistrar& operator=(IServerDataHandlerRegistrar&&) noexcept = default;

    virtual ~IServerDataHandlerRegistrar() = default;

    /// @brief Adds handler to handlers database
    /// @param id Input-struct id that handler is belongs to
    /// @param multicast Is it acceptable to have more than one handler to handle this Input-struct
    /// @param pService Pointer to service instance (service is aggregation object of handlers).
    ///     Note that nullptr is allowed.
    /// @param handler Handler instance
    /// @return Status of operation
    virtual Status registerHandler(const Id& id, bool multicast, Service* pService, IServerDataHandlerBase& handler) = 0;

    /// @brief Removes handler from handlers database
    /// @param id Input-struct id that handler was handling
    /// @param handler Handler instance
    virtual void unregisterHandler(const Id& id, IServerDataHandlerBase& handler) noexcept = 0;

    /// @brief Removes all handlers of particular service from handlers database
    /// @param pService Pointer to service instance that holds handlers that should be removed
    virtual void unregisterService(Service* pService) noexcept = 0;

    /// @brief Aquire all handlers that subscribed to handle Input-struct with given id
    /// @param id Input-struct id related to handlers
    /// @param handlers Container that would be filled with target handlers
    /// @return Status of operation.
    ///     If no handlers were found, Status::ErrorNoSuchHandler is returned.
    virtual Status aquireHandlers(const Id& id, RawVectorT<IServerDataHandlerBase*>& handlers) = 0;

    /// @brief Aquire single handler that is subsribed to handle Input-struct with given id
    /// @param id Input-struct id related to handler
    /// @param pHandler Pointer to target handler
    /// @return Status of operation.
    ///     If no handler was found, Status::ErrorNoSuchHandler is returned.
    ///     If there is more than one handler that handle this id, Status::ErrorMoreEntires is returned.
    virtual Status aquireHandler(const Id& id, IServerDataHandlerBase*& pHandler) noexcept = 0;

    /// @brief Release of ISereverDataHandler use
    /// @param id Input-struct id related to handler
    /// @param pHandler Pointer to target handler
    /// @return Status of operation
    /// @note Called by Server after handleData processing
    virtual void releaseHandler(IServerDataHandlerBase* pHandler) noexcept = 0;
};

} // namespace common_serialization::csp::messaging
