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

namespace common_serialization
{

/// @brief Constructor Allocator that not throwing
///     (as long as constructed objects don't do it)
/// @tparam _T Type of objects that allocator would allocate and construct
template<typename _T, IStorageAllocatorImpl _StorageAllocator = HeapAllocatorT>
class ConstructorNoexceptAllocator : public IAllocator<ConstructorAllocatorTraits<_T, _StorageAllocator>, ConstructorNoexceptAllocator<_T>>
{
public:
    using allocator_traits = ConstructorAllocatorTraits<_T>;
    using value_type = typename allocator_traits::value_type;
    using pointer = typename allocator_traits::pointer;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;
    using storage_allocator = _StorageAllocator;
    using constructor_allocator = typename allocator_traits::constructor_allocator;

    using allocator_interface_type = IAllocator<ConstructorAllocatorTraits<_T>, ConstructorNoexceptAllocator<_T>>;

    constexpr ConstructorNoexceptAllocator() = default;

    /// @brief Copy ctor
    /// @remark This overload only for compatibility
    /// @tparam _T2 Type of ojects that rhs allocator would allocate
    template <class _T2>
    explicit constexpr ConstructorNoexceptAllocator(const ConstructorNoexceptAllocator<_T2>&) noexcept {}

protected:
    friend allocator_interface_type;

    [[nodiscard]] CS_ALWAYS_INLINE constexpr pointer allocateImpl(size_type n) const noexcept
    {
        return static_cast<_T*>(memory_management::allocate(n * sizeof(_T)));
    }

    CS_ALWAYS_INLINE constexpr void deallocateImpl(pointer p) const noexcept
    {
        memory_management::deallocate(p);
    }

    CS_ALWAYS_INLINE constexpr void deallocateImpl(pointer p, size_type n) const noexcept
    {
        deallocate(p);
    }

    template<typename... _Args>
    CS_ALWAYS_INLINE constexpr Status constructImpl(pointer p, _Args&&... args) const
    {
        assert(p);

        new ((void*)p) value_type(std::forward<_Args>(args)...);
        return Status::NoError;
    }

    template<typename... _Args>
    CS_ALWAYS_INLINE constexpr Status constructImpl(pointer p, _Args&&... args) const
        requires Initable<value_type>
    {
        assert(p);

        new ((void*)p) value_type;

        if constexpr (sizeof...(_Args))
            return p->init(std::forward<_Args>(args)...);
        else
            return Status::NoError;
    }

    CS_ALWAYS_INLINE constexpr void destroyImpl(pointer p) const noexcept
    {
        p->~value_type();
    }

    CS_ALWAYS_INLINE constexpr size_type max_size_impl() const noexcept
    {
        return max_size_v;
    }

private:
    static constexpr size_type max_size_v = static_cast<size_type>(-1) / sizeof(_T);
};

} // namespace common_serialization
