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

namespace common_serialization
{

/// @brief Raw allocator that not throwing
template<typename _T>
class RawNoexceptAllocator : public IAllocator<RawAllocatorTraits<_T>, RawNoexceptAllocator<_T>>
{
public:
    using allocator_traits = RawAllocatorTraits<_T>;
    using value_type = typename allocator_traits::value_type;
    using pointer = typename allocator_traits::pointer;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;
    using constructor_allocator = typename allocator_traits::constructor_allocator;

    using interface_type = IAllocator<RawAllocatorTraits<_T>, RawNoexceptAllocator<_T>>;

    CS_ALWAYS_INLINE constexpr RawNoexceptAllocator() = default;

    /// @brief Copy ctor
    /// @remark This overload only for compatibility
    /// @tparam _T2 Type of ojects that rhs allocator would allocate
    template <class _T2>
    explicit CS_ALWAYS_INLINE constexpr RawNoexceptAllocator(const RawNoexceptAllocator<_T2>&) noexcept {}

protected:
    friend interface_type;

    [[nodiscard]] CS_ALWAYS_INLINE constexpr pointer allocateImpl(size_type n) const noexcept
    {
        return reinterpret_cast<pointer>(memory_management::allocate(n * sizeof(value_type)));
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
    CS_ALWAYS_INLINE constexpr Status constructImpl(pointer p, _Args&&... args) const noexcept
    {
        assert(p);

        new ((void*)p) value_type(std::forward<_Args>(args)...);
        return Status::NoError;
    }

    CS_ALWAYS_INLINE constexpr void destroyImpl(pointer p) const noexcept {}

    CS_ALWAYS_INLINE constexpr size_type max_sizeImpl() const noexcept
    {
        return max_size_v;
    }

private:
    static constexpr size_type max_size_v = static_cast<size_type>(-1) / sizeof(value_type);
};

} // namespace common_serialization
