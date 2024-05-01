/**
 * @file cslib/include/common_serialization/common_serialization.h
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

#ifdef USER_MODE // must be defined when c++ standard library is availible
#include <string.h>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <concepts>
#include <type_traits>
#include <limits>
#include <bit>
#include <shared_mutex>
#include <unordered_map>    // temporary header, must be replaced by internal map implementation
                            // to support environments without std libs, like OSes kernel modes
#endif // USER_MODE

#include "common_serialization/Status.h"
#include "common_serialization/NotCspInterfaceProcessing/Templates/Declarations.h"

#ifdef USER_MODE // must be defined when c++ standard library is availible

#include "common_serialization/Allocators/PlatformDependent/UserModeMemoryManagement.h"
#include "common_serialization/Concurrency/PlatformDependent/UserModeConcurency.h"

#else // !USER_MODE

#include "common_serialization/std_equivalents/std_equivalents.h"

#ifdef LINUX_KERNEL

#include "common_serialization/Allocators/PlatformDependent/LinuxKernelMemoryManagement.h"

#elif defined WINDOWS_KERNEL

#include "common_serialization/Allocators/PlatformDependent/WindowsKernelMemoryManagement.h"

#endif // LINUX_KERNEL || WINDOWS_KERNEL

#include "common_serialization/std_equivalents/new.h"

#endif // USER_MODE

#include "common_serialization/Types.h"
#include "common_serialization/Helpers.h"
#include "common_serialization/Concurrency/GuardRW.h"
#include "common_serialization/Allocators/Concepts.h"
#include "common_serialization/Allocators/RawKeeperAllocator.h"
#include "common_serialization/Containers/Walker.h"
#include "common_serialization/csp/ISerializable.h"
#include "common_serialization/csp/Macros.h"
#include "common_serialization/NotCspInterfaceProcessing/Others/Declarations.h"
#include "common_serialization/csp/processing/DataVersionConverters.h"
#include "common_serialization/csp/messaging/Client.h"
#include "common_serialization/csp/messaging/GenericDataServersRegistrar.h"
#include "common_serialization/csp/messaging/Server.h"
#include "common_serialization/csp/messaging/IDataHandler.h"
#include "common_serialization/NotCspInterfaceProcessing/Templates/Serialize.h"
#include "common_serialization/NotCspInterfaceProcessing/Templates/Deserialize.h"
#include "common_serialization/NotCspInterfaceProcessing/Others/Serialize.h"
#include "common_serialization/NotCspInterfaceProcessing/Others/Deserialize.h"
