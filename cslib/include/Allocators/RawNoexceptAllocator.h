/**
 * @file RawNoexceptAllocator.h
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
class RawNoexceptAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using constructor_allocator = std::false_type;

    constexpr RawNoexceptAllocator() noexcept {}
    template <class R>
    constexpr RawNoexceptAllocator(const RawNoexceptAllocator<R>&) noexcept {}

    [[nodiscard]] constexpr T* allocate(size_type n) const noexcept;
    constexpr void deallocate(T* p) const noexcept;
    constexpr void deallocate(T* p, size_type n) const noexcept;

    // construct and destroy in this class are present only for compotability reasons
    template<typename... Args>
    constexpr Status construct(T* p, Args&&... args) const noexcept;
    constexpr void destroy(T* p) const noexcept;

    constexpr size_type max_size() const noexcept;

private:
    static constexpr size_type max_size_v = static_cast<size_type>(-1) / sizeof(value_type);
};

template<typename T>
    requires std::is_trivially_copyable_v<T>
[[nodiscard]] constexpr T* RawNoexceptAllocator<T>::allocate(size_type n) const noexcept
{
    return reinterpret_cast<T*>(memory_management::raw_heap_allocate(n * sizeof(T)));
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawNoexceptAllocator<T>::deallocate(T* p) const noexcept
{
    memory_management::raw_heap_deallocate(p);
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawNoexceptAllocator<T>::deallocate(T* p, size_type n) const noexcept
{
    deallocate(p);
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
template<typename... Args>
constexpr Status RawNoexceptAllocator<T>::construct(T* p, Args&&... args) const noexcept
{
    assert(p);

    new ((void*)p) T(std::forward<Args>(args)...);
    return Status::kNoError;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawNoexceptAllocator<T>::destroy(T* p) const noexcept
{
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr RawNoexceptAllocator<T>::size_type RawNoexceptAllocator<T>::max_size() const noexcept
{
    return max_size_v;
}

} // namespace common_serialization
