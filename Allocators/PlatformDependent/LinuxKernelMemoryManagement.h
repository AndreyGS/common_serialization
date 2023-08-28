/**
 * @file LinuxKernelMemoryManagement.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin
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

namespace common_serialization
{

namespace memory_management
{

[[nodiscard]] inline void* raw_heap_allocate(size_t data_size_in_bytes) noexcept
{
    return kmalloc(data_size_in_bytes, GFP_KERNEL);
}

inline void raw_heap_deallocate(void* p) noexcept
{
    return kfree(p);
}

template<typename T, typename... Args>
constexpr inline void placement_new(T* p, Args&&... args) noexcept
{
    p->T::T(std::forward<Args>(args)...);
}

template<typename T>
constexpr inline void destroy(T* p) noexcept
{
    p->~T();
}

} // namespace memory_management

} // namespace common_serialization
