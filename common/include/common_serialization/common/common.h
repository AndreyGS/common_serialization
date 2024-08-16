/**
 * @file common_serialization/common/common.h
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

#if !defined WINDOWS_KERNEL && !defined LINUX_KERNEL
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <shared_mutex>
#include <map>
#include <unordered_map> 
#include <semaphore>
#include <latch>
#include <atomic>
#include <list>
#endif // !defined WINDOWS_KERNEL && !defined LINUX_KERNEL

#include <common_serialization/common/concepts.h>
#include <common_serialization/common/helpers.h>
#include <common_serialization/common/Status.h>
#include <common_serialization/common/std_equivalents.h>
#include <common_serialization/common/traits.h>
#include <common_serialization/common/Uuid.h>
#include <common_serialization/common/interfaces/IIoProcessor.h>
