/**
 * @file common_serialization/memory_management/platform_dependent/user_mode.h
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

#include <common_serialization/common/common.h>
#include <common_serialization/memory_management_interfaces/IStorageAllocator.h>

namespace common_serialization
{

/// @brief Raw heap allocator that not throwing
class HeapAllocator : public IStorageAllocator<HeapAllocator>
{
public:
    using storage_allocator_interface_type = IStorageAllocator<HeapAllocator>;

protected:
    friend storage_allocator_interface_type;

    CS_ALWAYS_INLINE [[nodiscard]] void* allocateImpl(size_t dataSizeInBytes) noexcept
    {
        return std::malloc(dataSizeInBytes);
    }

    CS_ALWAYS_INLINE void deallocateImpl(void* p) noexcept
    {
        std::free(p);
    }
};

} // namespace common_serialization
