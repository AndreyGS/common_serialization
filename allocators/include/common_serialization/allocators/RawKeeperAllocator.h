/**
 * @file common_serialization/allocators/RawKeeperAllocator.h
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
#include <common_serialization/allocator_interfaces/IStorageSetter.h>

namespace common_serialization
{

/// @brief Stateful allocator that using user supplied storage
/// @note This allocator is single threaded.
///     Any synchronization if need shall be used additionally.
/// @tparam T Type of objects that allocator would allocate and construct
template<typename T>
class RawKeeperAllocator 
    : public IAllocator<RawAllocatorTraits<T>, RawKeeperAllocator<T>>
    , public IStorageSetter<RawKeeperAllocator<T>>
{
public:
    using allocator_traits = RawAllocatorTraits<T>;
    using value_type = typename allocator_traits::value_type;
    using pointer = typename allocator_traits::pointer;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;
    using constructor_allocator = typename allocator_traits::constructor_allocator;

    using allocator_interface_type = IAllocator<RawAllocatorTraits<T>, RawKeeperAllocator<T>>;
    using storage_setter_interface_type = IStorageSetter<RawKeeperAllocator<T>>;

    AGS_CS_ALWAYS_INLINE constexpr RawKeeperAllocator() = default;

    /// @brief Init ctor
    /// @param p Pointer on storage
    /// @param memorySize Size of storage in value_type units
    AGS_CS_ALWAYS_INLINE constexpr RawKeeperAllocator(pointer p, size_type memorySize) noexcept
        : m_p(p), m_memorySize(memorySize)
    {
    }

    /// @brief Copy ctor
    /// @remark This overload only for compatibility and does not copying anything
    template <class R>
    AGS_CS_ALWAYS_INLINE constexpr RawKeeperAllocator(const RawKeeperAllocator<R>& rhs) noexcept 
    { 
        operator=(rhs); 
    }

    AGS_CS_ALWAYS_INLINE constexpr RawKeeperAllocator(const RawKeeperAllocator& rhs)
    {
        operator=<value_type>(rhs);
    }

    template <class R>
    AGS_CS_ALWAYS_INLINE constexpr RawKeeperAllocator(RawKeeperAllocator<R>&& rhs) noexcept
    {
        operator=(std::move(rhs));
    }

    AGS_CS_ALWAYS_INLINE constexpr RawKeeperAllocator(RawKeeperAllocator&& rhs) noexcept
    {
        operator=<value_type>(std::move(rhs));
    }

    /// @brief Copy assignment operator
    /// @remark Present only for compatibility and does not copying anything
    template <class R>
    AGS_CS_ALWAYS_INLINE constexpr RawKeeperAllocator& operator=(const RawKeeperAllocator<R>& rhs) noexcept
    {
        return *this;
    }

    /// @brief Copy assignment operator
    /// @remark Present only for compatibility and does not copying anything
    AGS_CS_ALWAYS_INLINE constexpr RawKeeperAllocator& operator=(const RawKeeperAllocator& rhs) noexcept
    {
        return operator=<value_type>(rhs);
    }

    template <class R>
    constexpr RawKeeperAllocator& operator=(RawKeeperAllocator<R>&& rhs) noexcept
    {
        m_p = static_cast<T*>(static_cast<void*>(rhs.allocate(rhs.max_size())));
        m_memorySize = rhs.max_size() * sizeof(R) / sizeof(T);
        rhs.setStorage(nullptr, 0);

        return *this;
    }

    AGS_CS_ALWAYS_INLINE constexpr RawKeeperAllocator& operator=(RawKeeperAllocator&& rhs) noexcept
    {
        return operator=<value_type>(std::move(rhs));
    }

protected:
    friend allocator_interface_type;
    friend storage_setter_interface_type;

    /// @brief Get pointer on storage if n*value_type <= sizeof(storage)
    /// @param n Number of elements of type T that storage must be capable to hold
    /// @return Pointer to storage, nullptr if current storage is not large enough
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] pointer allocateImpl(size_type n) const noexcept
    {
        return n <= m_memorySize && n != 0 ? m_p : nullptr;
    }

    /// @brief Does nothing
    /// @remark Present only for compatibility
    AGS_CS_ALWAYS_INLINE constexpr void deallocateImpl(pointer p) const noexcept
    {
    }

    /// @brief Does nothing
    /// @remark Present only for compatibility
    AGS_CS_ALWAYS_INLINE constexpr void deallocateImpl(pointer p, size_type n) const noexcept
    {

    }

    /// @brief Call ctor in range of internal storage
    /// @note If p is out of storage memory range or if it does not
    ///     aligned to sizeof(value_type) unit boundaries, returns error.
    /// @tparam ...Args Parameters types that go to ctor
    /// @param p Pointer to memory where object shall be created
    /// @param ...args Parameters that go to ctor
    /// @return Status of operation
    template<typename... Args>
    constexpr Status constructImpl(pointer p, Args&&... args) const noexcept
    {
        if (   
               p < m_p 
            || p + 1 > m_p + m_memorySize
            || (static_cast<uint8_t*>(static_cast<void*>(p)) - static_cast<uint8_t*>(static_cast<void*>(m_p))) % sizeof(value_type) != 0
        )
            return Status::ErrorInvalidArgument;

        new ((void*)p) value_type(std::forward<Args>(args)...);
        return Status::NoError;
    }

    /// @brief Does nothing
    /// @remark Present only for compatibility
    AGS_CS_ALWAYS_INLINE constexpr void destroyImpl(pointer p) const noexcept
    {
    }

    /// @brief Get size of storage in value_type units
    /// @return Size of storage in value_type units
    AGS_CS_ALWAYS_INLINE constexpr size_type max_size_impl() const noexcept
    {
        return m_memorySize;
    }

    constexpr bool setStorageImpl(pointer p, size_t size) noexcept
    {
        if (p && !size || !p && size)
        {
            m_p = nullptr;
            m_memorySize = 0;
            return false;
        }

        m_p = reinterpret_cast<pointer>(p);
        m_memorySize = size;

        return true;
    }

private:
    pointer m_p{ nullptr };
    size_type m_memorySize{ 0 };
};

} // namespace common_serialization
