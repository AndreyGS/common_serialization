/**
 * @file common_serialization.h
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

 // After cmake support of modules will be not experemental (with 3.28.0)
 // this header should be transformed to module, with respective lib changes

#pragma once

#define RUN(x)                                                                  \
{                                                                               \
    if (Status status = (x); !ST_SUCCESS(status))                               \
        return status;                                                          \
}

#ifdef USER_MODE // must be defined when c++ standard library is availible
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <concepts>
#include <type_traits>
#include <limits>
#include <unordered_map>

#include "../Allocators/PlatformDependent/UserModeMemoryManagement.h"

#else // !USER_MODE

#include "../std_equivalents/std_equivalents.h"

#ifdef LINUX_KERNEL

#include "../Allocators/PlatformDependent/LinuxKernelMemoryManagement.h"

#elif defined WINDOWS_KERNEL

#include "../Allocators/PlatformDependent/WindowsKernelMemoryManagement.h"

#endif // LINUX_KERNEL || WINDOWS_KERNEL

//#include "../std_equivalents/new.h"

#endif // USER_MODE

#include "../CsHelpers.h"
#include "../Allocators/AllocatorConcepts.h"
#include "../Allocators/RawKeeperAllocator.h"
#include "../Containers/Walker.h"
#include "../Serialization/ISerializable.h"
#include "../Serialization/SerializationSpecialStructs.h"

#undef RUN
