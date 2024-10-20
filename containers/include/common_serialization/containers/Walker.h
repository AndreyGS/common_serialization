/**
 * @file common_serialization/containers/Walker.h
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

#include <common_serialization/containers/Vector.h>

namespace common_serialization
{

template<typename T, IAllocationManagerImpl AllocationManager = CtorStratAllocationManagerT<T>>
class Walker
{
public:
    using value_type = typename Vector<T, AllocationManager>::value_type;
    using size_type = typename Vector<T, AllocationManager>::size_type;

    using iterator = VectorIterator<Vector<T, AllocationManager>>;
    using const_iterator = ConstVectorIterator<Vector<T, AllocationManager>>;

    using prefer_init_against_ctor = std::true_type;

    Walker() = default;

    explicit AGS_CS_ALWAYS_INLINE constexpr Walker(const Vector<T, AllocationManager>& rhs);
    explicit AGS_CS_ALWAYS_INLINE constexpr Walker(Vector<T, AllocationManager>&& rhs) noexcept;

    constexpr Status init(const Walker& rhs);
    constexpr Status init(Walker&& rhs) noexcept;
    constexpr Status init(const Vector<T, AllocationManager>& rhs);
    constexpr Status init(Vector<T, AllocationManager>&& rhs) noexcept;

    // only set data size, no default values are set
    constexpr Status setSize(size_type n) noexcept
        requires std::is_trivially_copyable_v<T>;
    
    AGS_CS_ALWAYS_INLINE constexpr Status reserve(size_type n);
    constexpr Status reserve_from_current_offset(size_type n);

    constexpr Status pushBack(const T& value);
    constexpr Status pushBack(T&& value);
    constexpr Status pushBackN(const T* p, size_type n);

    template<typename V>
    constexpr Status pushBackArithmeticValue(V value)
        requires std::is_same_v<T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>);

    constexpr Status replace(const T* p, size_type n, size_type offset);

    constexpr Status insert(const T* p, size_type n, size_type offset);
    template<typename ItSrc>
    constexpr Status insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin, iterator* pDestEnd = nullptr);

    constexpr Status erase(size_type offset, size_type n);
    constexpr Status erase(iterator destBegin, iterator destEnd);

    AGS_CS_ALWAYS_INLINE constexpr Status write(const T* p, size_type n);
    
    // destination (p) must be initialized (for non pod-types)
    constexpr Status read(T* p, size_type n, size_type* pNRead = nullptr);

    template<typename V>
    constexpr Status readArithmeticValue(V& value) noexcept
        requires std::is_same_v<T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>);

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] T* data() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const T* data() const noexcept;
    constexpr [[nodiscard]] T& operator[](size_type offset);
    constexpr [[nodiscard]] const T& operator[](size_type offset) const;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type size() const noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type max_size() const noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type capacity() const noexcept;

    AGS_CS_ALWAYS_INLINE constexpr void clear() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr void invalidate() noexcept;

    // you shall free memory returned by this method manually
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] T* release() noexcept;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] AllocationManager& getAllocationManager() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const AllocationManager& getAllocationManager() const noexcept;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] Vector<T, AllocationManager>& getVector() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const Vector<T, AllocationManager>& getVector() const noexcept;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type tell() const noexcept;
    constexpr Status seek(size_type offset) noexcept;

private:
    void setValidOffset(size_type offset) noexcept;

    Vector<T, AllocationManager> m_vector;
    size_type m_offset{ 0 };
};

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Walker<T, AllocationManager>::Walker(const Vector<T, AllocationManager>& rhs)
{
    init(rhs);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Walker<T, AllocationManager>::Walker(Vector<T, AllocationManager>&& rhs) noexcept
{
    init(std::move(rhs));
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::init(const Walker& rhs)
{
    if (this != &rhs)
    {
        AGS_CS_RUN(m_vector.init(rhs.m_vector));
        m_offset = rhs.m_offset;
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::init(Walker&& rhs) noexcept
{
    if (this != &rhs)
    {
        AGS_CS_RUN(m_vector.init(std::move(rhs.m_vector)));
        m_offset = rhs.m_offset;
        rhs.m_offset = 0;
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::init(const Vector<T, AllocationManager>& rhs)
{
    if (&this->getVector() != &rhs)
    {
        AGS_CS_RUN(m_vector.init(rhs));
        m_offset = 0;
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::init(Vector<T, AllocationManager>&& rhs) noexcept
{
    if (&this->getVector() != &rhs)
    {
        AGS_CS_RUN(m_vector.init(std::move(rhs)));
        m_offset = 0;
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::setSize(size_type n) noexcept
    requires std::is_trivially_copyable_v<T>
{
    Status status = m_vector.setSize(n);
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::reserve(size_type n)
{
    return m_vector.reserve(n);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::reserve_from_current_offset(size_type n)
{
    if (m_offset + n < m_offset)
        return Status::ErrorOverflow;
    else
        return m_vector.reserve(m_offset + n);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::pushBack(const T& value)
{
    Status status = m_vector.pushBack(value);
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::pushBack(T&& value)
{
    Status status = m_vector.pushBack(std::move(value));
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::pushBackN(const T* p, size_type n)
{
    Status status = m_vector.pushBackN(p, n);
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl AllocationManager>
template<typename V>
constexpr Status Walker<T, AllocationManager>::pushBackArithmeticValue(V value)
    requires std::is_same_v<T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>)
{
    Status status = m_vector.pushBackArithmeticValue(value);
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::replace(const T* p, size_type n, size_type offset)
{
    setValidOffset(offset);
    AGS_CS_RUN(m_vector.replace(p, n, offset));
    m_offset += n;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
template<typename V>
constexpr Status Walker<T, AllocationManager>::readArithmeticValue(V& value) noexcept
    requires std::is_same_v<T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>)
{
    if (sizeof(V) + m_offset <= size())
    {
        value = *static_cast<const V*>(static_cast<const void*>(data() + m_offset));
        m_offset += sizeof(V);
        return Status::NoError;
    }
    else
        return Status::ErrorOverflow;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::insert(const T* p, size_type n, size_type offset)
{
    size_type oldSize = size();
    setValidOffset(offset);

    AGS_CS_RUN(m_vector.insert(p, n, offset));
    m_offset += size() - oldSize;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
template<typename ItSrc>
constexpr Status Walker<T, AllocationManager>::insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin, iterator* pDestEnd)
{
    size_type oldSize = size();
    size_type newOffset = destBegin - m_vector.begin();
    setValidOffset(newOffset);

    AGS_CS_RUN(m_vector.insert(srcBegin, srcEnd, destBegin, pDestEnd));
    m_offset += size() - oldSize;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::erase(size_type offset, size_type n)
{
    setValidOffset(offset);
    return m_vector.erase(offset, n);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::erase(iterator destBegin, iterator destEnd)
{
    size_type offset = destBegin - m_vector.begin();
    setValidOffset(offset);
    return m_vector.erase(destBegin, destEnd);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::write(const T* p, size_type n)
{
    return replace(p, n, m_offset);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::read(T* p, size_type n, size_type* pNRead)
{
    T* pNew = nullptr;

    AGS_CS_RUN(m_vector.copyN(m_offset, n, p, &pNew));
    
    size_type nReal = pNew - p;
    m_offset += nReal;

    if (pNRead)
        *pNRead = nReal;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr T* Walker<T, AllocationManager>::data() noexcept
{
    return m_vector.data();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr const T* Walker<T, AllocationManager>::data() const noexcept
{
    return m_vector.data();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr T& Walker<T, AllocationManager>::operator[](size_type offset)
{ 
    setValidOffset(offset);
    return m_vector[offset];
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr const T& Walker<T, AllocationManager>::operator[](size_type offset) const
{
    setValidOffset(offset);
    return m_vector[offset];
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Walker<T, AllocationManager>::size_type Walker<T, AllocationManager>::size() const noexcept
{
    return m_vector.size();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Walker<T, AllocationManager>::size_type Walker<T, AllocationManager>::max_size() const noexcept
{
    return m_vector.max_size();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Walker<T, AllocationManager>::size_type Walker<T, AllocationManager>::capacity() const noexcept
{
    return m_vector.capacity();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr void Walker<T, AllocationManager>::clear() noexcept
{
    m_vector.clear(), m_offset = 0;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr void Walker<T, AllocationManager>::invalidate() noexcept
{
    m_vector.invalidate(), m_offset = 0;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr T* Walker<T, AllocationManager>::release() noexcept
{
    return m_offset = 0, m_vector.release();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr AllocationManager& Walker<T, AllocationManager>::getAllocationManager() noexcept
{
    return m_vector.getAllocationManager();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr const AllocationManager& Walker<T, AllocationManager>::getAllocationManager() const noexcept
{
    return m_vector.getAllocationManager();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Vector<T, AllocationManager>& Walker<T, AllocationManager>::getVector() noexcept
{
    return m_vector;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr const Vector<T, AllocationManager>& Walker<T, AllocationManager>::getVector() const noexcept
{
    return m_vector;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Walker<T, AllocationManager>::size_type Walker<T, AllocationManager>::tell() const noexcept
{
    return m_offset;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Walker<T, AllocationManager>::seek(size_type offset) noexcept
{
    setValidOffset(offset);
    return offset <= size() ? Status::NoError : Status::ErrorOverflow;
}

template<typename T, IAllocationManagerImpl AllocationManager>
void Walker<T, AllocationManager>::setValidOffset(size_type offset) noexcept
{
    m_offset = offset < size() ? offset : size();
}

} // namespace common_serialization
