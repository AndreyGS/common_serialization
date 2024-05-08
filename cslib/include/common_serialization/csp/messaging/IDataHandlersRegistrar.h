/**
 * @file cslib/include/common_serialization/csp/messaging/IIDataServersKeeper.h
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

#include "common_serialization/csp/ISerializable.h"
#include "common_serialization/Containers/Vector.h"
#include "common_serialization/csp/messaging/IDataHandlerTraits.h"

namespace common_serialization::csp::messaging
{

template<SdContainers T>
class IDataHandlerBase;

/// @brief Registrar of servers that can process InOutData CSP requests
template<SdContainers _Sdcs = traits::DefaultSdContainers>
class IDataHandlersRegistrar
{
public:
    using Sdcs = _Sdcs;

    IDataHandlersRegistrar() = default;

    IDataHandlersRegistrar(const IDataHandlersRegistrar&) = delete;
    IDataHandlersRegistrar& operator=(const IDataHandlersRegistrar&) = delete;

    IDataHandlersRegistrar(IDataHandlersRegistrar&&) noexcept = default;
    IDataHandlersRegistrar& operator=(IDataHandlersRegistrar&&) noexcept = default;

    virtual ~IDataHandlersRegistrar() = default;

    /// @brief Adds handler to handlers database
    /// @param id Input-struct id that handler is belongs to
    /// @param multicast Is it acceptable to have more than one handler to handle this Input-struct
    /// @param pInstance Pointer on handler instance
    /// @return Status of operation
    virtual Status addHandler(const Id& id, bool multicast, IDataHandlerBase<Sdcs>* pInstance) = 0;

    /// @brief Removes handler from handlers database
    /// @param id Input-struct id that handler was handling
    /// @param pInstance Handler instance that must be deleted
    virtual void removeHandler(const Id& id, IDataHandlerBase<Sdcs>* pInstance) noexcept = 0;

    /// @brief Find all handlers that subscribed to handle Input-struct with given id
    /// @param id Input-struct id related to handlers
    /// @param handlers Container that would be filled with target handlers
    /// @return Status of operation.
    ///     If no handlers were found, Status::kErrorNoSuchHandler is returned.
    virtual Status findHandlers(const Id& id, Vector<IDataHandlerBase<Sdcs>*, RawStrategicAllocatorHelper<IDataHandlerBase<Sdcs>*>>& handlers) const noexcept = 0;

    /// @brief Find single handler that is subsribed to handle Input-struct with given id
    /// @param id Input-struct id related to handler
    /// @param pHandler Pointer on target handler
    /// @return Status of operation.
    ///     If no handler was found, Status::kErrorNoSuchHandler is returned.
    ///     If there is more than one handler that handle this id, Status::kErrorMoreEntires is returned.
    virtual Status findHandler(const Id& id, IDataHandlerBase<Sdcs>*& pHandler) const noexcept = 0;
};

} // namespace common_serialization::csp::messaging
