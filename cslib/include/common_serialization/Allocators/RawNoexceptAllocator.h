/**
 * @file cslib/include/common_serialization/Allocators/RawNoexceptAllocator.h
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

#include "common_serialization/Allocators/PlatformDependent/switch.h"

namespace common_serialization
{

/// @brief Raw allocator that not throwing
/// @tparam _T Type of objects that allocator would allocate and construct
template<typename _T>
    requires std::is_trivially_copyable_v<_T>
class RawNoexceptAllocator
{
public:
    using value_type = _T;
    using pointer = value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using constructor_allocator = std::false_type;

    /// @brief Default ctor
    constexpr RawNoexceptAllocator() = default;

    /// @brief Copy ctor
    /// @remark This overload only for compatibility
    /// @tparam R Type of ojects that rhs allocator would allocate
    template <class R>
    explicit constexpr RawNoexceptAllocator(const RawNoexceptAllocator<R>&) noexcept {}

    /// @brief Copy ctor
    /// @remark This overload only for compatibility
    constexpr RawNoexceptAllocator(const RawNoexceptAllocator&) = default;

    /// @brief Allocate storage with bytes_size = n*sizeof(_T)
    /// @param n Number of elements of type _T that storage must be capable to hold
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    [[nodiscard]] constexpr pointer allocate(size_type n) const noexcept;

    /// @brief Frees storage pointed by p
    /// @param p Pointer to memory that shall be freed
    constexpr void deallocate(pointer p) const noexcept;

    /// @brief Frees storage pointed by p
    /// @remark This overload only for compatibility
    /// @param p Pointer to memory that shall be freed
    /// @param n Size of storage (not used)
    constexpr void deallocate(pointer p, size_type n) const noexcept;

    /// @brief Call ctor with args on memory pointed by p
    /// @remark This method only for compatibility
    /// @tparam ...Args Parameters types that go to ctor
    /// @param p Pointer to memory where object shall be created
    /// @param ...args Parameters that go to ctor
    /// @return Status of operation
    template<typename... _Args>
    constexpr Status construct(pointer p, _Args&&... args) const noexcept;

    /// @brief Does nothing
    /// @param p This overload only for compatibility
    constexpr void destroy(pointer p) const noexcept;

    /// @brief Get maximum number of objects of type _T that allocator can allocate
    /// @return Maximum number of objects
    constexpr size_type max_size() const noexcept;

private:
    static constexpr size_type max_size_v = static_cast<size_type>(-1) / sizeof(value_type);
};

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
[[nodiscard]] constexpr _T* RawNoexceptAllocator<_T>::allocate(size_type n) const noexcept
{
    return reinterpret_cast<pointer>(memory_management::raw_heap_allocate(n * sizeof(value_type)));
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr void RawNoexceptAllocator<_T>::deallocate(pointer p) const noexcept
{
    memory_management::raw_heap_deallocate(p);
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr void RawNoexceptAllocator<_T>::deallocate(pointer p, size_type n) const noexcept
{
    deallocate(p);
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
template<typename... _Args>
constexpr Status RawNoexceptAllocator<_T>::construct(pointer p, _Args&&... args) const noexcept
{
    assert(p);

    new ((void*)p) value_type(std::forward<_Args>(args)...);
    return Status::kNoError;
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr void RawNoexceptAllocator<_T>::destroy(pointer p) const noexcept
{
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr typename RawNoexceptAllocator<_T>::size_type RawNoexceptAllocator<_T>::max_size() const noexcept
{
    return max_size_v;
}

} // namespace common_serialization
