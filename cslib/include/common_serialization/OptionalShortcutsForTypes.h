/**
 * @file cslib/include/common_serialization/OptionalShortcutsForTypes.h
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

#include "common_serialization/common_serialization.h"


/*
namespace cs
{

using Status = common_serialization::Status;
using Uuid   = common_serialization::Uuid;
using Dummy  = common_serialization::Dummy;

template<typename T>
using ConstructorNoexceptAllocator = common_serialization::ConstructorNoexceptAllocator<T>;

template<typename T, common_serialization::IAllocator Allocator = ConstructorNoexceptAllocator<T>>
using StrategicAllocatorHelper = common_serialization::StrategicAllocatorHelper<T, Allocator>;

template<typename T, typename AllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T>>>
using Vector = common_serialization::Vector<T, AllocatorHelper>;
template<typename T, typename AllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T>>>
using Walker = common_serialization::Walker<T, AllocatorHelper>;

template<typename X, typename T>
using GetCrtpMainType = common_serialization::GetCrtpMainType<X, T>;

template<typename T>
using ISerializable = common_serialization::csp::ISerializable<T>;

using protocol_version_t = common_serialization::csp::protocol_version_t;
using interface_version_t = common_serialization::csp::interface_version_t;

using DataProcessor = common_serialization::csp::processing::DataProcessor;

} // namespace cs
*/

