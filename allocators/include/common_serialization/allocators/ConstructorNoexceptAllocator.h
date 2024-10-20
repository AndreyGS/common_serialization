/**
 * @file common_serialization/allocators/ConstructorNoexceptAllocator.h
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

#include <common_serialization/allocator_interfaces/IAllocator.h>
#include <common_serialization/allocators/allocators_config.h>

namespace common_serialization
{

/// @brief Constructor Allocator that not throwing
///     (as long as constructed objects don't do it)
/// @tparam T Type of objects that allocator would allocate and construct
template<typename T>
class ConstructorNoexceptAllocator : public IAllocator<ConstructorAllocatorTraits<T>, ConstructorNoexceptAllocator<T>>
{
public:
    using allocator_traits = ConstructorAllocatorTraits<T>;
    using value_type = typename allocator_traits::value_type;
    using pointer = typename allocator_traits::pointer;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;
    using constructor_allocator = typename allocator_traits::constructor_allocator;

    using allocator_interface_type = IAllocator<ConstructorAllocatorTraits<T>, ConstructorNoexceptAllocator<T>>;

    constexpr ConstructorNoexceptAllocator() = default;

    /// @brief Copy ctor
    /// @remark This overload only for compatibility
    /// @tparam T2 Type of ojects that rhs allocator would allocate
    template <class T2>
    explicit constexpr ConstructorNoexceptAllocator(const ConstructorNoexceptAllocator<T2>&) noexcept {}

protected:
    friend allocator_interface_type;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] pointer allocateImpl(size_type n) const noexcept
    {
        return static_cast<T*>(HeapAllocatorT().allocate(n * sizeof(T)));
    }

    AGS_CS_ALWAYS_INLINE constexpr void deallocateImpl(pointer p) const noexcept
    {
        HeapAllocatorT().deallocate(p);
    }

    AGS_CS_ALWAYS_INLINE constexpr void deallocateImpl(pointer p, size_type n) const noexcept
    {
        deallocate(p);
    }

    template<typename... Args>
    AGS_CS_ALWAYS_INLINE constexpr Status constructImpl(pointer p, Args&&... args) const
    {
        assert(p);

        new ((void*)p) value_type(std::forward<Args>(args)...);
        return Status::NoError;
    }

    template<typename... Args>
    AGS_CS_ALWAYS_INLINE constexpr Status constructImpl(pointer p, Args&&... args) const
        requires (InitableBySpecialArgs<value_type, Args...> && value_type::prefer_init_against_ctor::value)
    {
        assert(p);

        new ((void*)p) value_type;

        if constexpr (sizeof...(Args))
            return p->init(std::forward<Args>(args)...);
        else
            return Status::NoError;
    }

    AGS_CS_ALWAYS_INLINE constexpr void destroyImpl(pointer p) const noexcept
    {
        p->~value_type();
    }

    AGS_CS_ALWAYS_INLINE constexpr size_type max_size_impl() const noexcept
    {
        return max_size_v;
    }

private:
    static constexpr size_type max_size_v = static_cast<size_type>(-1) / sizeof(T);
};

} // namespace common_serialization
