/**
 * @file common_serialization/memory_management_interfaces/IStorageAllocator.h
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

namespace common_serialization
{

/// @brief Interface for allocation of raw storage
/// @tparam _PsAllocator Most derived class (instance type)
template<typename _StorageAllocator>
class IStorageAllocator
{
public:
    /// @brief Allocate memory
    /// @param dataSizeInBytes Requested size
    /// @return Pointer to allocated storage
    [[nodiscard]] CS_ALWAYS_INLINE void* allocate(size_t dataSizeInBytes) noexcept
    {
        return static_cast<_StorageAllocator*>(this)->allocateImpl(dataSizeInBytes);
    }

    /// @brief Deallocate memory
    /// @param p Pointer to previously allocated storage
    CS_ALWAYS_INLINE void deallocate(void* p) noexcept
    {
        static_cast<_StorageAllocator*>(this)->deallocateImpl(p);
    }
};

template<typename _StorageAllocator>
concept IStorageAllocatorImpl = std::is_base_of_v<IStorageAllocator<normalize_t<_StorageAllocator>>, normalize_t<_StorageAllocator>>;

} // namespace common_serialization
