/**
 * @file cslib/include/common_serialization/std_equivalents/new.h
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

// new
[[nodiscard]] constexpr void* operator new(size_t data_size_in_bytes) noexcept
{
    return common_serialization::memory_management::raw_heap_allocate(data_size_in_bytes);
}

// new[]
[[nodiscard]] constexpr void* operator new[](size_t data_size_in_bytes) noexcept
{
    return common_serialization::memory_management::raw_heap_allocate(data_size_in_bytes);
}

// new nothrow
[[nodiscard]] constexpr void* operator new(size_t data_size_in_bytes, const std::nothrow_t&) noexcept
{
    return common_serialization::memory_management::raw_heap_allocate(data_size_in_bytes);
}

// new[] nothrow
[[nodiscard]] constexpr void* operator new[](size_t data_size_in_bytes, const std::nothrow_t&) noexcept
{
    return common_serialization::memory_management::raw_heap_allocate(data_size_in_bytes);
}

// placement new
[[nodiscard]] constexpr inline void* operator new(uint64_t, void* p) noexcept
{
    return p;
}

// placement new[]
[[nodiscard]] constexpr inline void* operator new[](uint64_t, void* p) noexcept
{
    return p;
}

// delete
constexpr void operator delete(void* p) noexcept
{
    common_serialization::memory_management::raw_heap_deallocate(p);
}

// delete[]
constexpr void operator delete[](void* p) noexcept
{
    common_serialization::memory_management::raw_heap_deallocate(p);
}

// delete nothrow
constexpr void operator delete(void* p, const std::nothrow_t&) noexcept
{
    common_serialization::memory_management::raw_heap_deallocate(p);
}

// delete[] nothrow
constexpr void operator delete[](void* p, const std::nothrow_t&) noexcept
{
    common_serialization::memory_management::raw_heap_deallocate(p);
}

// placement delete
constexpr inline void operator delete(void*, void* ) noexcept
{
}

// placement delete[]
constexpr inline void operator delete[](void*, void* ) noexcept
{
}
