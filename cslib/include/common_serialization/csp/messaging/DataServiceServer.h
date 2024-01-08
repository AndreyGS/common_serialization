/**
 * @file cslib/include/common_serialization/csp/messaging/DataServiceServer.h
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

#include "common_serialization/csp/messaging/IDataServer.h"
#include "common_serialization/csp/messaging/ServiceStructs.h"

// Service structs not support version conversion and must always be the same
// Set is completely depend on protocol version

namespace common_serialization::csp::messaging
{

template<typename T>
concept DataServiceServerTraits = requires
{
    { T::fillInterfacesList(*(new Vector<traits::InterfaceProperties>)) };
};

/// @brief Predefined data server that answers on CSP clients service requests
/// @tparam Traits Class that support interface necessary for DataServiceServer work
template<DataServiceServerTraits Traits>
class DataServiceServer
    : IStaticDataServer<DataServiceServer<Traits>, GetInterfaceProperties<>, OutGetInterfaceProperties<>, false>
    , IStaticDataServer<DataServiceServer<Traits>, GetInterfacesList<>, InterfacesList<>, false>
{
public: 
    static Vector<traits::InterfaceProperties>& getInterfacesList()
    {
        static Vector<traits::InterfaceProperties> list;

        Traits::fillInterfacesList(list);

        return list;
    }

    static Status handleDataStatic(const GetInterfaceProperties<>& input, Vector<GenericPointerKeeper>* pUnmanagedPointers, OutGetInterfaceProperties<>& output)
    {
        output.properties.id = input.id;
        output.properties.version = traits::kInterfaceVersionUndefined;

        for (auto& item : getInterfacesList())
            if (input.id == item.id)
            {
                output.properties.version = item.version;
                break;
            }

        return Status::kNoError;
    }

    static Status handleDataStatic(const GetInterfacesList<>& input, Vector<GenericPointerKeeper>* pUnmanagedPointers, InterfacesList<>& output)
    {
        return output.list.init(getInterfacesList());
    }
};

} // namespace common_serialization::csp::messaging
