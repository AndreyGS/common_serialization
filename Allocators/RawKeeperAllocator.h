/**
 * @file RawKeeperAllocator.h
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

class RawKeeperAllocator
{
public:
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    constexpr RawKeeperAllocator() noexcept {}
    constexpr RawKeeperAllocator(void* p, size_type memorySize) noexcept;

    constexpr void setStorage(void* p, size_type memorySize) noexcept;

    [[nodiscard]] inline void* allocate(size_type data_size_in_bytes) const noexcept;
    inline void deallocate(void* p) const noexcept;

    constexpr size_type max_size() const noexcept;

private:
    void* m_p = nullptr;
    size_type m_memorySize = 0;
};

constexpr RawKeeperAllocator::RawKeeperAllocator(void* p, size_type memorySize) noexcept
    : m_p(p), m_memorySize(memorySize)
{
}

constexpr void RawKeeperAllocator::setStorage(void* p, size_type memorySize) noexcept
{
    m_p = p;
    m_memorySize = memorySize;
}

[[nodiscard]] inline void* RawKeeperAllocator::allocate(size_type data_size_in_bytes) const noexcept
{
    return data_size_in_bytes <= m_memorySize ? m_p : nullptr;
}

inline void RawKeeperAllocator::deallocate(void* p) const noexcept
{
}

constexpr RawKeeperAllocator::size_type RawKeeperAllocator::max_size() const noexcept
{
    return m_memorySize;
}

} // namespace common_serialization
