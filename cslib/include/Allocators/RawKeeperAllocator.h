/**
 * @file RawKeeperAllocator.h
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

namespace common_serialization
{

template<typename T>
    requires std::is_trivially_copyable_v<T>
class RawKeeperAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using constructor_allocator = std::false_type;

    constexpr RawKeeperAllocator() noexcept {}
    constexpr RawKeeperAllocator(T* p, size_type memorySize) noexcept;

    template <class R>
    constexpr RawKeeperAllocator(const RawKeeperAllocator<R>& rhs) noexcept { operator=(rhs); }
    constexpr RawKeeperAllocator(const RawKeeperAllocator& rhs) { return operator=<T>(rhs); }

    template <class R>
    constexpr RawKeeperAllocator(RawKeeperAllocator<R>&& rhs) noexcept;
    constexpr RawKeeperAllocator(RawKeeperAllocator&& rhs) noexcept { operator=<T>(std::move(rhs)); }

    template <class R>
    constexpr RawKeeperAllocator& operator=(const RawKeeperAllocator<R>&) noexcept { return *this; }
    constexpr RawKeeperAllocator& operator=(const RawKeeperAllocator& rhs) noexcept { return operator=<T>(rhs); }

    template <class R>
    constexpr RawKeeperAllocator& operator=(RawKeeperAllocator<R>&& rhs) noexcept;
    constexpr RawKeeperAllocator& operator=(RawKeeperAllocator&& rhs) noexcept { return operator=<T>(std::move(rhs)); }

    constexpr void setStorage(T* p, size_type memorySize) noexcept;
    //constexpr void setSCtorage(const T* p, size_type memorySize) noexcept;

    [[nodiscard]] constexpr T* allocate(size_type data_size_in_bytes) const noexcept;
    constexpr void deallocate(T* p) const noexcept;
    constexpr void deallocate(T* p, size_type n) const noexcept;

    // construct and destroy in this class are present only for compotability reasons
    template<typename... Args>
    constexpr Status construct(T* p, Args&&... args) const noexcept;
    constexpr void destroy(T* p) const noexcept;

    constexpr size_type max_size() const noexcept;

private:
    T* m_p{ nullptr };
    size_type m_memorySize{ 0 };
};

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr RawKeeperAllocator<T>::RawKeeperAllocator(T* p, size_type memorySize) noexcept
    : m_p(p), m_memorySize(memorySize)
{
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
template <class R>
constexpr RawKeeperAllocator<T>::RawKeeperAllocator(RawKeeperAllocator<R>&& rhs) noexcept
{
    operator=(std::move(rhs));
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
template <class R>
constexpr RawKeeperAllocator<T>& RawKeeperAllocator<T>::operator=(RawKeeperAllocator<R>&& rhs) noexcept
{
    m_p = rhs.m_p;
    rhs.m_p = nullptr;
    m_memorySize = rhs.m_memorySize *sizeof(R) / sizeof(T);
    rhs.m_memorySize = 0;

    return *this;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawKeeperAllocator<T>::setStorage(T* p, size_type memorySize) noexcept
{
    if (p && !memorySize || !p && memorySize)
        return;

    m_p = p;
    m_memorySize = memorySize;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
[[nodiscard]] constexpr T* RawKeeperAllocator<T>::allocate(size_type n) const noexcept
{
    return n <= m_memorySize ? m_p : nullptr;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawKeeperAllocator<T>::deallocate(T* p) const noexcept
{
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawKeeperAllocator<T>::deallocate(T* p, size_type n) const noexcept
{
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
template<typename... Args>
constexpr Status RawKeeperAllocator<T>::construct(T* p, Args&&... args) const noexcept
{
    new ((void*)p) T(std::forward<Args>(args)...);
    return Status::kNoError;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawKeeperAllocator<T>::destroy(T* p) const noexcept
{
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr RawKeeperAllocator<T>::size_type RawKeeperAllocator<T>::max_size() const noexcept
{
    return m_memorySize;
}

} // namespace common_serialization