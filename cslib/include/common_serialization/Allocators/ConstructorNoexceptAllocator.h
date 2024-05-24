/**
 * @file cslib/include/common_serialization/Allocators/ConstructorNoexceptAllocator.h
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

/// @brief Constructor Allocator that not throwing
///     (as long as constructed objects don't do it)
/// @tparam _T Type of objects that allocator would allocate and construct
template<typename _T>
class ConstructorNoexceptAllocator 
{
public:
    using value_type = _T;
    using pointer = value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using constructor_allocator = std::true_type;

    /// @brief Default ctor
    constexpr ConstructorNoexceptAllocator() = default;

    /// @brief Copy ctor
    /// @remark This overload only for compatibility
    /// @tparam R Type of ojects that rhs allocator would allocate
    template <class R>
    explicit constexpr ConstructorNoexceptAllocator(const ConstructorNoexceptAllocator<R>&) noexcept {}

    /// @brief Copy ctor
    /// @remark This overload only for compatibility
    constexpr ConstructorNoexceptAllocator(const ConstructorNoexceptAllocator&) = default;

    /// @brief Allocate storage with bytes_size = n*sizeof(value_type)
    /// @param n Number of elements of type value_type that storage must be capable to hold
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
    /// @tparam ..._Args Parameters types that go to ctor
    /// @param p Pointer to memory where object shall be created
    /// @param ...args Parameters that go to ctor
    /// @return Status of operation
    template<typename... _Args>
    constexpr Status construct(pointer p, _Args&&... args) const;

    /// @brief Call default ctor on memory pointed by p
    ///     and then call init() method of value_type if args pack not empty
    /// @tparam ..._Args Parameters types that go to init() method
    /// @param p Pointer to memory where object shall be created
    /// @param ...args Parameters that go to init() method
    /// @return Status of operation
    template<typename... _Args>
    constexpr Status construct(pointer p, _Args&&... args) const
        requires Initable<value_type>;

    /// @brief Call destructor on object pointed by p
    /// @param p Pointer to object that shall be destroyed
    constexpr void destroy(pointer p) const noexcept;

    /// @brief Get maximum number of objects of type value_type that allocator can allocate
    /// @return Maximum number of objects
    constexpr size_type max_size() const noexcept;

private:
    static constexpr size_type max_size_v = static_cast<size_type>(-1) / sizeof(_T);
};

template<typename _T>
[[nodiscard]] constexpr _T* ConstructorNoexceptAllocator<_T>::allocate(size_type n) const noexcept
{
    return static_cast<_T*>(memory_management::raw_heap_allocate(n * sizeof(_T)));
}

template<typename _T>
constexpr void ConstructorNoexceptAllocator<_T>::deallocate(pointer p) const noexcept
{
    memory_management::raw_heap_deallocate(p);
}

template<typename _T>
constexpr void ConstructorNoexceptAllocator<_T>::deallocate(pointer p, size_type n) const noexcept
{
    deallocate(p);
}

template<typename _T>
template<typename... _Args>
constexpr Status ConstructorNoexceptAllocator<_T>::construct(pointer p, _Args&&... args) const
{
    assert(p);

    new ((void*)p) value_type(std::forward<_Args>(args)...);
    return Status::NoError;
}

template<typename _T>
template<typename... _Args>
constexpr Status ConstructorNoexceptAllocator<_T>::construct(pointer p, _Args&&... args) const
    requires Initable<value_type>
{
    assert(p);

    new ((void*)p) value_type;

    if constexpr (sizeof...(_Args))
        return p->init(std::forward<_Args>(args)...);
    else
        return Status::NoError;
}

template<typename _T>
constexpr void ConstructorNoexceptAllocator<_T>::destroy(pointer p) const noexcept
{
    p->~value_type();
}

template<typename _T>
constexpr typename ConstructorNoexceptAllocator<_T>::size_type ConstructorNoexceptAllocator<_T>::max_size() const noexcept
{
    return max_size_v;
}

} // namespace common_serialization
