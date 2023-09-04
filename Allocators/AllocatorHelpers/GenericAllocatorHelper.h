/**
 * @file GenericAllocatorHelper.h
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

#include "Allocators/ConstructorNoexceptAllocator.h"
#include "InterfaceAllocatorHelper.h"
#include "helpers.h"

namespace common_serialization
{

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
class GenericAllocatorHelperImpl : public InterfaceAllocatorHelper<T, Allocator, AllocatorHelper>
{
public:
    using size_type = typename InterfaceAllocatorHelper<T, Allocator, AllocatorHelper>::size_type;

protected:
    friend InterfaceAllocatorHelper<T, Allocator, AllocatorHelper>;

    constexpr GenericAllocatorHelperImpl() : InterfaceAllocatorHelper<T, Allocator, AllocatorHelper>() { }

    template<typename... Args>
    [[nodiscard]] constexpr T* allocateAndConstructImpl(size_type requestedN, size_type* allocatedN, Args&&... args) const;
    [[nodiscard]] constexpr T* allocateImpl(size_type n, size_type* allocatedN) const;
    [[nodiscard]] constexpr T* allocateStrictImpl(size_type n) const;

    template<typename... Args>
    constexpr void constructImpl(T* p, Args&&... args) const;
    template<typename... Args>
    constexpr void constructNImpl(T* p, size_type n, Args&&... args) const;

    constexpr void copyImpl(T* dest, const T* src, size_type n) const;
    constexpr void copyNoOverlapImpl(T* dest, const T* src, size_type n) const;

    constexpr void copyAssignImpl(T* dest, const T* src, size_type n) const;
    constexpr void copyAssignNoOverlapImpl(T* dest, const T* src, size_type n) const;

    constexpr void moveImpl(T* dest, T* src, size_type n) const;
    constexpr void moveNoOverlapImpl(T* dest, T* src, size_type n) const;

    constexpr void destroyAndDeallocateImpl(T* p, size_type n) const noexcept;
    constexpr void deallocateImpl(T* p) const noexcept;
    constexpr void destroyImpl(T* p) const noexcept;
    constexpr void destroyNImpl(T* p, size_type n) const noexcept;

    constexpr size_type max_size_impl() const noexcept;
};

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
template<typename... Args>
[[nodiscard]] constexpr T* GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::allocateAndConstructImpl(size_type requestedN, size_type* allocatedN, Args&&... args) const
{
    assert(allocatedN);

    T* p = this->allocate(requestedN * sizeof(T), allocatedN);

    if constexpr (ConstructorAllocator<Allocator>)
        if (p)
            this->construct_n(p, requestedN, std::forward<Args>(args)...);

    return p;
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
[[nodiscard]] constexpr T* GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::allocateImpl(size_type requestedN, size_type* allocatedN) const
{
    T* p = static_cast<T*>(this->getAllocator().allocate(sizeof(T) * requestedN));

    *allocatedN = p ? requestedN : 0;

    return p;
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
[[nodiscard]] constexpr T* GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::allocateStrictImpl(size_type n) const
{
    return static_cast<T*>(this->getAllocator().allocate(sizeof(T) * n));
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
template<typename... Args>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::constructImpl(T* p, Args&&... args) const
{
    if constexpr (ConstructorAllocator<Allocator>)
        if (p)
            this->getAllocator().construct(p, std::forward<Args>(args)...);
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
template<typename... Args>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::constructNImpl(T* p, size_type n, Args&&... args) const
{
    if constexpr (ConstructorAllocator<Allocator>)
        if (p)
            for (size_type i = 0; i < n; ++i)
                this->getAllocator().construct(p + i, args...);
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::copyImpl(T* dest, const T* src, size_type n) const
{
    assert(!n || dest && src);

    if (helpers::areRegionsOverlap(dest, src, n) && dest > src)
    {
        if constexpr (ConstructorAllocator<Allocator>)
            for (size_type i = n - 1; i != static_cast<size_type>(-1); --i)
                this->getAllocator().construct(dest + i, *(src + i));
        else if constexpr (RawAllocator<Allocator>)
            memmove(dest, src, n * sizeof(T));
    }
    else
        copyNoOverlapImpl(dest, src, n);

}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::copyNoOverlapImpl(T* dest, const T* src, size_type n) const
{
    assert(!n || dest && src);

    if constexpr (ConstructorAllocator<Allocator>)
        for (size_type i = 0; i < n; ++i)
            this->getAllocator().construct(dest + i, *(src + i));
    
    else if constexpr (RawAllocator<Allocator>)
        memcpy(dest, src, n * sizeof(T));
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::copyAssignImpl(T* dest, const T* src, size_type n) const
{
    assert(!n || dest && src);

    if (helpers::areRegionsOverlap(dest, src, n) && dest > src)
    {
        if constexpr (ConstructorAllocator<Allocator>)
            for (size_type i = n - 1; i != static_cast<size_type>(-1); --i)
                *(dest + i) = *(src + i);
        else if constexpr (RawAllocator<Allocator>)
            memmove(dest, src, n * sizeof(T));
    }
    else
        copyAssignNoOverlapImpl(dest, src, n);

}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::copyAssignNoOverlapImpl(T* dest, const T* src, size_type n) const
{
    assert(!n || dest && src);

    if constexpr (ConstructorAllocator<Allocator>)
        for (size_type i = 0; i < n; ++i)
            *(dest + i) = *(src + i);

    else if constexpr (RawAllocator<Allocator>)
        memcpy(dest, src, n * sizeof(T));
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::moveImpl(T* dest, T* src, size_type n) const
{
    assert(!n || dest && src);

    if (helpers::areRegionsOverlap(dest, src, n) && dest > src)
    {
        if constexpr (ConstructorAllocator<Allocator>)
            for (size_type i = n - 1; i != static_cast<size_type>(-1); --i)
                this->getAllocator().construct(dest + i, std::move(*(src + i)));
        else if constexpr (RawAllocator<Allocator>)
            memmove(dest, src, n * sizeof(T));
    }
    else
        moveNoOverlapImpl(dest, src, n);
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::moveNoOverlapImpl(T* dest, T* src, size_type n) const
{
    assert(!n || dest && src);

    if constexpr (ConstructorAllocator<Allocator>)
        for (size_type i = 0; i < n; ++i)
            this->getAllocator().construct(dest + i, std::move(*(src + i)));

    else if constexpr (RawAllocator<Allocator>)
        memcpy(dest, src, n * sizeof(T));
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::destroyAndDeallocateImpl(T* p, size_type n) const noexcept
{
    if constexpr (ConstructorAllocator<Allocator>)
        this->destroyN(p, n);
    this->deallocate(p);
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::deallocateImpl(T* p) const noexcept
{
    this->getAllocator().deallocate(p);
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::destroyImpl(T* p) const noexcept
{
    if constexpr (ConstructorAllocator<Allocator>)
        if (p)
            this->getAllocator().destroy(p);
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr void GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::destroyNImpl(T* p, size_type n) const noexcept
{   
    if constexpr (ConstructorAllocator<Allocator>)
        if (p)
            for (size_type i = 0; i < n; ++i)
                this->getAllocator().destroy(p + i);
}

template<typename T, BasicAllocator Allocator, typename AllocatorHelper>
constexpr GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::size_type GenericAllocatorHelperImpl<T, Allocator, AllocatorHelper>::max_size_impl() const noexcept
{
    return this->getAllocator().max_size();
}

template<typename T, BasicAllocator Allocator>
class GenericAllocatorHelper : public GenericAllocatorHelperImpl<T, Allocator, GenericAllocatorHelper<T, Allocator>>
{
public:
    using value_type = typename InterfaceAllocatorHelper<T, Allocator, GenericAllocatorHelperImpl<T, Allocator, GenericAllocatorHelper<T, Allocator>>>::value_type;
    using pointer = typename InterfaceAllocatorHelper<T, Allocator, GenericAllocatorHelperImpl<T, Allocator, GenericAllocatorHelper<T, Allocator>>>::pointer;
    using size_type = typename GenericAllocatorHelperImpl<T, Allocator, GenericAllocatorHelper<T, Allocator>>::size_type;
    using difference_type = typename GenericAllocatorHelperImpl<T, Allocator, GenericAllocatorHelper<T, Allocator>>::difference_type;
};

} // namespace common_serialization
