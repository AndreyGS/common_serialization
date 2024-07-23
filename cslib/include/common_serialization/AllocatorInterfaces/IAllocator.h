/**
 * @file cslib/include/common_serialization/AllocatorInterfaces/IAllocator.h
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

#include "common_serialization/AllocatorInterfaces/IAllocatorTraits.h"

namespace common_serialization
{

/// @brief Interface of Allocator
/// @tparam _AllocatorTraits Allocator traits
/// @tparam _Allocator Most derived class (instance type)
template<IAllocatorTraitsImpl _AllocatorTraits, typename _Allocator>
class IAllocator
{
public:
    using allocator_traits = _AllocatorTraits;
    using value_type = typename allocator_traits::value_type;
    using pointer = typename allocator_traits::pointer;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;
    using constructor_allocator = typename allocator_traits::constructor_allocator;

    /// @brief Allocate storage with bytes_size = n*sizeof(value_type)
    /// @param n Number of elements of type _T that storage must be capable to hold
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    [[nodiscard]] CS_ALWAYS_INLINE constexpr pointer allocate(size_type n) const noexcept
    {
        return static_cast<const _Allocator*>(this)->allocateImpl(n);
    }

    /// @brief Frees storage pointed by p
    /// @param p Pointer to memory that shall be freed
    CS_ALWAYS_INLINE constexpr void deallocate(pointer p) const noexcept
    {
        return static_cast<const _Allocator*>(this)->deallocateImpl(p);
    }

    /// @brief Frees storage pointed by p
    /// @remark This overload only for compatibility
    /// @param p Pointer to memory that shall be freed
    /// @param n Size of storage (not used)
    CS_ALWAYS_INLINE constexpr void deallocate(pointer p, size_type n) const noexcept
    {
        return static_cast<const _Allocator*>(this)->deallocateImpl(p, n);
    }

    /// @brief Call ctor with args on memory pointed by p
    /// @remark This method only for compatibility
    /// @tparam ...Args Parameters types that go to ctor
    /// @param p Pointer to memory where object shall be created
    /// @param ...args Parameters that go to ctor
    /// @return Status of operation
    template<typename... _Args>
    CS_ALWAYS_INLINE constexpr Status construct(pointer p, _Args&&... args) const noexcept
    {
        return static_cast<const _Allocator*>(this)->constructImpl(p, std::forward<_Args>(args)...);
    }

    /// @brief Call destructor on object pointed by p
    /// @param p Pointer to object that shall be destroyed
    CS_ALWAYS_INLINE constexpr void destroy(pointer p) const noexcept
    {
        return static_cast<const _Allocator*>(this)->destroyImpl(p);
    }

    /// @brief Get maximum number of objects of value_type that allocator can allocate
    /// @return Maximum number of objects
    CS_ALWAYS_INLINE constexpr size_type max_size() const noexcept
    {
        return static_cast<const _Allocator*>(this)->max_size_impl();
    }
};

template<typename _T>
concept IAllocatorImpl = std::is_base_of_v<IAllocator<typename _T::allocator_traits, normalize_t<_T>>, normalize_t<_T>>;

} // namespace common_serialization
