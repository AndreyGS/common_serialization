/**
 * @file cslib/include/common_serialization/Allocators/RawKeeperAllocator.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

/// @brief Stateful allocator that using user supplied storage
/// @note This allocator is single threaded.
///     Any synchronization if need shall be used additionally.
/// @tparam T Type of objects that allocator would allocate and construct
template<typename T>
    requires std::is_trivially_copyable_v<T>
class RawKeeperAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using constructor_allocator = std::false_type;

    /// @brief Default constructor
    constexpr RawKeeperAllocator() noexcept {}

    /// @brief Init constructor
    /// @param p Pointer on storage
    /// @param memorySize Size of storage in T units
    constexpr RawKeeperAllocator(T* p, size_type memorySize) noexcept;

    /// @brief Copy constructor
    /// @remark This overload only for compatibility and does nothing
    /// @tparam R Type of ojects that rhs allocator is allocate
    /// @param rhs Another RawKeeperAllocator object
    template <class R>
    constexpr RawKeeperAllocator(const RawKeeperAllocator<R>& rhs) noexcept { operator=(rhs); }

    /// @brief Copy constructor
    /// @remark This overload only for compatibility and does nothing
    /// @param rhs Another RawKeeperAllocator object
    constexpr RawKeeperAllocator(const RawKeeperAllocator& rhs) { return operator=<T>(rhs); }

    /// @brief Move constructor
    /// @tparam R Type of ojects that rhs allocator is allocate
    /// @param rhs Another RawKeeperAllocator object
    template <class R>
    constexpr RawKeeperAllocator(RawKeeperAllocator<R>&& rhs) noexcept;

    /// @brief Move constructor
    /// @param rhs Another RawKeeperAllocator object
    constexpr RawKeeperAllocator(RawKeeperAllocator&& rhs) noexcept { operator=<T>(std::move(rhs)); }

    /// @brief Copy assignment operator
    /// @remark Present only for compatibility and does not copying anything
    /// @tparam R Type of ojects that rhs allocator is allocate
    template <class R>
    constexpr RawKeeperAllocator& operator=(const RawKeeperAllocator<R>&) noexcept { return *this; }

    /// @brief Copy assignment operator
    /// @remark Present only for compatibility and does not copying anything
    /// @param rhs Another RawKeeperAllocator object
    /// @return *this
    constexpr RawKeeperAllocator& operator=(const RawKeeperAllocator& rhs) noexcept { return operator=<T>(rhs); }

    /// @brief Move assignment operator
    /// @tparam R Type of ojects that rhs allocator is allocate
    /// @param rhs Another RawKeeperAllocator object
    /// @return *this
    template <class R>
    constexpr RawKeeperAllocator& operator=(RawKeeperAllocator<R>&& rhs) noexcept;

    /// @brief Move assignment operator
    /// @param rhs Another RawKeeperAllocator object
    /// @return *this
    constexpr RawKeeperAllocator& operator=(RawKeeperAllocator&& rhs) noexcept { return operator=<T>(std::move(rhs)); }

    /// @brief Init with storage
    /// @param p Pointer to storage
    /// @param memorySize Size of storage
    constexpr void setStorage(T* p, size_type memorySize) noexcept;
    
    /// @brief Get pointer on storage if n*T <= sizeof(storage)
    /// @param n Number of elements of type T that storage must be capable to hold
    /// @return Pointer to storage, nullptr if current storage is not large enough
    [[nodiscard]] constexpr T* allocate(size_type n) const noexcept;

    /// @brief Does nothing
    /// @remark Present only for compatibility
    /// @param p Pointer to storage
    constexpr void deallocate(T* p) const noexcept;

    /// @brief Does nothing
    /// @remark Present only for compatibility
    /// @param p Pointer to storage
    /// @param n Number of elements
    constexpr void deallocate(T* p, size_type n) const noexcept;

    /// @brief Call constructor with args on memory pointed by p
    /// @note If p is out of storage memory range or if it does not
    ///     aligned to sizeof(T) unit boundaries, returns error.
    /// @remark This method only for compatibility
    /// @tparam ...Args Parameters types that go to constructor
    /// @param p Pointer to memory where object shall be created
    /// @param ...args Parameters that go to constructor
    /// @return Status of operation
    template<typename... Args>
    constexpr Status construct(T* p, Args&&... args) const noexcept;

    /// @brief Does nothing
    /// @param p This overload only for compatibility
    constexpr void destroy(T* p) const noexcept;

    /// @brief Get size of storage in T units
    /// @return Size of storage in T units
    constexpr size_type max_size() const noexcept;

private:
    T* m_p{ nullptr };
    size_type m_memorySize{ 0 };
};

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr RawKeeperAllocator<T>::RawKeeperAllocator(T* p, size_type memorySize) noexcept
    : m_p(p), m_memorySize(memorySize)
{
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
template <class R>
constexpr RawKeeperAllocator<T>::RawKeeperAllocator(RawKeeperAllocator<R>&& rhs) noexcept
{
    operator=(std::move(rhs));
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
template <class R>
constexpr RawKeeperAllocator<T>& RawKeeperAllocator<T>::operator=(RawKeeperAllocator<R>&& rhs) noexcept
{
    m_p = rhs.m_p;
    rhs.m_p = nullptr;
    m_memorySize = rhs.m_memorySize *sizeof(R) / sizeof(T);
    rhs.m_memorySize = 0;

    return *this;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawKeeperAllocator<T>::setStorage(T* p, size_type memorySize) noexcept
{
    if (p && !memorySize || !p && memorySize)
        return;

    m_p = p;
    m_memorySize = memorySize;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
[[nodiscard]] constexpr T* RawKeeperAllocator<T>::allocate(size_type n) const noexcept
{
    return n <= m_memorySize ? m_p : nullptr;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawKeeperAllocator<T>::deallocate(T* p) const noexcept
{
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawKeeperAllocator<T>::deallocate(T* p, size_type n) const noexcept
{
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
template<typename... Args>
constexpr Status RawKeeperAllocator<T>::construct(T* p, Args&&... args) const noexcept
{
    if (p < m_p || p + 1 > m_p + m_memorySize || (p - m_p) % sizeof(T) != 0)
        return Status::kErrorInvalidArgument;

    new ((void*)p) T(std::forward<Args>(args)...);
    return Status::kNoError;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr void RawKeeperAllocator<T>::destroy(T* p) const noexcept
{
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
constexpr RawKeeperAllocator<T>::size_type RawKeeperAllocator<T>::max_size() const noexcept
{
    return m_memorySize;
}

} // namespace common_serialization
