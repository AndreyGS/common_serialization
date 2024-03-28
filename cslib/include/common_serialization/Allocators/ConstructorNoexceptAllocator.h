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

#include "common_serialization/Concepts.h"

namespace common_serialization
{

/// @brief Constructor Allocator that not throwing
///     (as long as constructed objects don't do it)
/// @tparam T Type of objects that allocator would allocate and construct
template<typename T>
class ConstructorNoexceptAllocator 
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using constructor_allocator = std::true_type;

    /// @brief Default constructor
    constexpr ConstructorNoexceptAllocator() noexcept {}

    /// @brief Copy constructor
    /// @remark This overload only for compatibility
    /// @tparam R Type of ojects that rhs allocator would allocate
    template <class R>
    constexpr ConstructorNoexceptAllocator(const ConstructorNoexceptAllocator<R>&) noexcept {}

    /// @brief Allocate storage with bytes_size = n*sizeof(T)
    /// @param n Number of elements of type T that storage must be capable to hold
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    [[nodiscard]] constexpr T* allocate(size_type n) const noexcept;

    /// @brief Frees storage pointed by p
    /// @param p Pointer to memory that shall be freed
    constexpr void deallocate(T* p) const noexcept;

    /// @brief Frees storage pointed by p
    /// @remark This overload only for compatibility
    /// @param p Pointer to memory that shall be freed
    /// @param n Size of storage (not used)
    constexpr void deallocate(T* p, size_type n) const noexcept;

    /// @brief Call constructor with args on memory pointed by p
    /// @tparam ...Args Parameters types that go to constructor
    /// @param p Pointer to memory where object shall be created
    /// @param ...args Parameters that go to constructor
    /// @return Status of operation
    template<typename... Args>
    constexpr Status construct(T* p, Args&&... args) const noexcept;

    /// @brief Call default constructor on memory pointed by p
    ///     and then call init() method of T if args pack not empty
    /// @tparam ...Args Parameters types that go to init() method
    /// @param p Pointer to memory where object shall be created
    /// @param ...args Parameters that go to init() method
    /// @return Status of operation
    template<typename... Args>
    constexpr Status construct(T* p, Args&&... args) const noexcept
        requires Initable<T>;

    /// @brief Call destructor on object pointed by p
    /// @param p Pointer to object that shall be destroyed
    constexpr void destroy(T* p) const noexcept;

    /// @brief Get maximum number of objects of type T that allocator can allocate
    /// @return Maximum number of objects
    constexpr size_type max_size() const noexcept;

private:
    static constexpr size_type max_size_v = static_cast<size_type>(-1) / sizeof(T);
};

template<typename T>
[[nodiscard]] constexpr T* ConstructorNoexceptAllocator<T>::allocate(size_type n) const noexcept
{
    return static_cast<T*>(memory_management::raw_heap_allocate(n * sizeof(T)));
}

template<typename T>
constexpr void ConstructorNoexceptAllocator<T>::deallocate(T* p) const noexcept
{
    memory_management::raw_heap_deallocate(p);
}

template<typename T>
constexpr void ConstructorNoexceptAllocator<T>::deallocate(T* p, size_type n) const noexcept
{
    deallocate(p);
}

template<typename T>
template<typename... Args>
constexpr Status ConstructorNoexceptAllocator<T>::construct(T* p, Args&&... args) const noexcept
{
    assert(p);

    new ((void*)p) T(std::forward<Args>(args)...);
    return Status::kNoError;
}

template<typename T>
template<typename... Args>
constexpr Status ConstructorNoexceptAllocator<T>::construct(T* p, Args&&... args) const noexcept
    requires Initable<T>
{
    assert(p);

    new ((void*)p) T;

    if constexpr (sizeof...(Args))
        return p->init(std::forward<Args>(args)...);
    else
        return Status::kNoError;
}

template<typename T>
constexpr void ConstructorNoexceptAllocator<T>::destroy(T* p) const noexcept
{
    p->~T();
}

template<typename T>
constexpr typename ConstructorNoexceptAllocator<T>::size_type ConstructorNoexceptAllocator<T>::max_size() const noexcept
{
    return max_size_v;
}

} // namespace common_serialization
