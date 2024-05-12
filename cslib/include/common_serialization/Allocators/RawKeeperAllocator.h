/**
 * @file cslib/include/common_serialization/Allocators/RawKeeperAllocator.h
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

/// @brief Stateful allocator that using user supplied storage
/// @note This allocator is single threaded.
///     Any synchronization if need shall be used additionally.
/// @tparam _T Type of objects that allocator would allocate and construct
template<typename _T>
    requires std::is_trivially_copyable_v<_T>
class RawKeeperAllocator
{
public:
    using value_type = _T;
    using pointer = value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using constructor_allocator = std::false_type;

    /// @brief Default ctor
    constexpr RawKeeperAllocator() = default;

    /// @brief Init ctor
    /// @param p Pointer on storage
    /// @param memorySize Size of storage in _T units
    constexpr RawKeeperAllocator(pointer p, size_type memorySize) noexcept;

    /// @brief Copy ctor
    /// @remark This overload only for compatibility and does nothing
    /// @tparam R Type of ojects that rhs allocator is allocate
    /// @param rhs Another RawKeeperAllocator object
    template <class _R>
    constexpr RawKeeperAllocator(const RawKeeperAllocator<_R>& rhs) noexcept { operator=(rhs); }

    /// @brief Copy ctor
    /// @remark This overload only for compatibility and does nothing
    /// @param rhs Another RawKeeperAllocator object
    constexpr RawKeeperAllocator(const RawKeeperAllocator& rhs) { operator=<value_type>(rhs); }

    /// @brief Move ctor
    /// @tparam R Type of ojects that rhs allocator is allocate
    /// @param rhs Another RawKeeperAllocator object
    template <class _R>
    constexpr RawKeeperAllocator(RawKeeperAllocator<_R>&& rhs) noexcept;

    /// @brief Move ctor
    /// @param rhs Another RawKeeperAllocator object
    constexpr RawKeeperAllocator(RawKeeperAllocator&& rhs) noexcept { operator=<value_type>(std::move(rhs)); }

    /// @brief Copy assignment operator
    /// @remark Present only for compatibility and does not copying anything
    /// @tparam R Type of ojects that rhs allocator is allocate
    template <class _R>
    constexpr RawKeeperAllocator& operator=(const RawKeeperAllocator<_R>&) noexcept { return *this; }

    /// @brief Copy assignment operator
    /// @remark Present only for compatibility and does not copying anything
    /// @param rhs Another RawKeeperAllocator object
    /// @return *this
    constexpr RawKeeperAllocator& operator=(const RawKeeperAllocator& rhs) noexcept { return operator=<value_type>(rhs); }

    /// @brief Move assignment operator
    /// @tparam R Type of ojects that rhs allocator is allocate
    /// @param rhs Another RawKeeperAllocator object
    /// @return *this
    template <class _R>
    constexpr RawKeeperAllocator& operator=(RawKeeperAllocator<_R>&& rhs) noexcept;

    /// @brief Move assignment operator
    /// @param rhs Another RawKeeperAllocator object
    /// @return *this
    constexpr RawKeeperAllocator& operator=(RawKeeperAllocator&& rhs) noexcept { return operator=<value_type>(std::move(rhs)); }

    /// @brief Init with storage
    /// @param p Pointer to storage
    /// @param memorySize Size of storage
    constexpr void setStorage(pointer p, size_type memorySize) noexcept;
    
    /// @brief Get pointer on storage if n*value_type <= sizeof(storage)
    /// @param n Number of elements of type _T that storage must be capable to hold
    /// @return Pointer to storage, nullptr if current storage is not large enough
    [[nodiscard]] constexpr pointer allocate(size_type n) const noexcept;

    /// @brief Does nothing
    /// @remark Present only for compatibility
    /// @param p Pointer to storage
    constexpr void deallocate(pointer p) const noexcept;

    /// @brief Does nothing
    /// @remark Present only for compatibility
    /// @param p Pointer to storage
    /// @param n Number of elements
    constexpr void deallocate(pointer p, size_type n) const noexcept;

    /// @brief Call ctor with args on memory pointed by p
    /// @note If p is out of storage memory range or if it does not
    ///     aligned to sizeof(value_type) unit boundaries, returns error.
    /// @remark This method only for compatibility
    /// @tparam ..._Args Parameters types that go to ctor
    /// @param p Pointer to memory where object shall be created
    /// @param ...args Parameters that go to ctor
    /// @return Status of operation
    template<typename... _Args>
    constexpr Status construct(pointer p, _Args&&... args) const noexcept;

    /// @brief Does nothing
    /// @param p This overload only for compatibility
    constexpr void destroy(pointer p) const noexcept;

    /// @brief Get size of storage in value_type units
    /// @return Size of storage in value_type units
    constexpr size_type max_size() const noexcept;

private:
    pointer m_p{ nullptr };
    size_type m_memorySize{ 0 };
};

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr RawKeeperAllocator<_T>::RawKeeperAllocator(pointer p, size_type memorySize) noexcept
    : m_p(p), m_memorySize(memorySize)
{
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
template <class R>
constexpr RawKeeperAllocator<_T>::RawKeeperAllocator(RawKeeperAllocator<R>&& rhs) noexcept
{
    operator=(std::move(rhs));
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
template <class R>
constexpr RawKeeperAllocator<_T>& RawKeeperAllocator<_T>::operator=(RawKeeperAllocator<R>&& rhs) noexcept
{
    m_p = rhs.m_p;
    rhs.m_p = nullptr;
    m_memorySize = rhs.m_memorySize *sizeof(R) / sizeof(_T);
    rhs.m_memorySize = 0;

    return *this;
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr void RawKeeperAllocator<_T>::setStorage(pointer p, size_type memorySize) noexcept
{
    if (p && !memorySize || !p && memorySize)
        return;

    m_p = p;
    m_memorySize = memorySize;
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
[[nodiscard]] constexpr _T* RawKeeperAllocator<_T>::allocate(size_type n) const noexcept
{
    return n <= m_memorySize ? m_p : nullptr;
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr void RawKeeperAllocator<_T>::deallocate(pointer p) const noexcept
{
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr void RawKeeperAllocator<_T>::deallocate(pointer p, size_type n) const noexcept
{
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
template<typename... _Args>
constexpr Status RawKeeperAllocator<_T>::construct(pointer p, _Args&&... args) const noexcept
{
    if (   
           p < m_p 
        || p + 1 > m_p + m_memorySize
        || (static_cast<uint8_t*>(static_cast<void*>(p)) - static_cast<uint8_t*>(static_cast<void*>(m_p))) % sizeof(value_type) != 0
    )
        return Status::kErrorInvalidArgument;

    new ((void*)p) value_type(std::forward<_Args>(args)...);
    return Status::kNoError;
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr void RawKeeperAllocator<_T>::destroy(pointer p) const noexcept
{
}

template<typename _T>
    requires std::is_trivially_copyable_v<_T>
constexpr typename RawKeeperAllocator<_T>::size_type RawKeeperAllocator<_T>::max_size() const noexcept
{
    return m_memorySize;
}

} // namespace common_serialization
