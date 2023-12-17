/**
 * @file cslib/include/common_serialization/CSP/MessagingDataServiceServer.h
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

#include "common_serialization/CSP/MessagingServiceStructs.h"

// Service structs not support version conversion and must always be the same
// Set is completely depend on protocol version

namespace common_serialization::csp::messaging
{

template<typename T>
concept DataServiceServerBase = requires
{
    { T::fillInterfacesList(*(new Vector<traits::InterfaceProperties>)) };
};

template<DataServiceServerBase Base>
class DataServiceServer
    : Base
    , IStaticDataServer<DataServiceServer<Base>, GetInterfaceProperties<>, OurGetInterfaceProperties<>, false>
    , IStaticDataServer<DataServiceServer<Base>, GetInterfacesList<>, InterfacesList<>, false>
{
public: 
    static Vector<traits::InterfaceProperties>& getInterfacesList()
    {
        static Vector<traits::InterfaceProperties> list;

        Base::fillInterfacesList(list);

        return list;
    }

    static Status handleDataStatic(const GetInterfaceProperties<>& input, Vector<GenericPointerKeeper>* unmanagedPointers, OurGetInterfaceProperties<>& output)
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

    static Status handleDataStatic(const GetInterfacesList<>& input, Vector<GenericPointerKeeper>* unmanagedPointers, InterfacesList<>& output)
    {
        return output.list.init(getInterfacesList());
    }
};

} // namespace common_serialization::csp::messaging
