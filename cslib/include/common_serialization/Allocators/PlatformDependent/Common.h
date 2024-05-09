/**
 * @file cslib/include/common_serialization/Allocators/PlatformDependent/Common.h
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

#if defined WINDOWS_KERNEL

#include "common_serialization/Allocators/PlatformDependent/WindowsKernelMemoryManagement.h"

#elif defined LINUX_KERNEL

#include "common_serialization/Allocators/PlatformDependent/LinuxKernelMemoryManagement.h"

#else // USER_MODE

#include "common_serialization/Allocators/PlatformDependent/UserModeMemoryManagement.h"

#endif // defined WINDOWS_KERNEL, defined LINUX_KERNEL


#if !defined CS_NO_STD_NEW_DELETE_REPLACEMENT && (defined WINDOWS_KERNEL || defined LINUX_KERNEL)

#include "common_serialization/Allocators/PlatformDependent/NewDeleteReplacements.h"

#endif // defined WINDOWS_KERNEL || defined LINUX_KERNEL
