/**
 * @file common_serialization/allocation_managers/typedefs.h
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

#ifndef AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_TYPEDEFS_HEADER_PATH

#include <common_serialization/allocation_managers/GenericAllocationManager.h>
#include <common_serialization/allocation_managers/StrategicAllocationManager.h>

namespace common_serialization
{

template<IAllocatorImpl Allocator>
using GenericAllocationManagerT = GenericAllocationManager<Allocator>;

template<IAllocatorImpl Allocator>
using StrategicAllocationManagerT = StrategicAllocationManager<Allocator>;


template<typename T>
using RawGenAllocationManagerT = GenericAllocationManager<RawNoexceptAllocatorT<T>>;

template<typename T>
using RawKeeperGenAllocationManagerT = GenericAllocationManager<RawKeeperAllocatorT<T>>;

template<typename T>
using CtorGenAllocationManagerT = GenericAllocationManager<ConstructorNoexceptAllocatorT<T>>;

template<typename T>
using RawStratAllocationManagerT = StrategicAllocationManager<RawNoexceptAllocatorT<T>>;

template<typename T>
using CtorStratAllocationManagerT = StrategicAllocationManager<ConstructorNoexceptAllocatorT<T>>;

} // namespace common_serialization

#endif // #ifndef AGS_CS_CUSTOM_ALLOCATOR_MANAGERS_TYPEDEFS_HEADER_PATH
