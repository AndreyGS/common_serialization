/**
 * @file cslib/include/common_serialization/Containers/Walker.h
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

#include "common_serialization/Containers/Vector.h"

namespace common_serialization
{

template<typename T, IAllocationManagerImpl _AllocationManager = ConstrStratAllocationManagerT<T>>
class Walker
{
public:
    using value_type = typename Vector<T, _AllocationManager>::value_type;
    using size_type = typename Vector<T, _AllocationManager>::size_type;

    using iterator = VectorIterator<Vector<T, _AllocationManager>>;
    using const_iterator = ConstVectorIterator<Vector<T, _AllocationManager>>;

    Walker() = default;

    explicit constexpr Walker(const Vector<T, _AllocationManager>& rhs);
    explicit constexpr Walker(Vector<T, _AllocationManager>&& rhs) noexcept;

    constexpr Status init(const Walker& rhs);
    constexpr Status init(Walker&& rhs) noexcept;
    constexpr Status init(const Vector<T, _AllocationManager>& rhs);
    constexpr Status init(Vector<T, _AllocationManager>&& rhs) noexcept;

    // only set data size, no default values are set
    constexpr Status setSize(size_type n) noexcept
        requires std::is_trivially_copyable_v<T>;
    
    constexpr Status reserve(size_type n);
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

    constexpr Status write(const T* p, size_type n);
    
    // destination (p) must be initialized (for non pod-types)
    constexpr Status read(T* p, size_type n, size_type* pNRead = nullptr);

    template<typename V>
    constexpr Status readArithmeticValue(V& value) noexcept
        requires std::is_same_v<T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>);

    [[nodiscard]] constexpr T* data() noexcept;
    [[nodiscard]] constexpr const T* data() const noexcept;
    [[nodiscard]] constexpr T& operator[](size_type offset);
    [[nodiscard]] constexpr const T& operator[](size_type offset) const;

    [[nodiscard]] constexpr size_type size() const noexcept;
    [[nodiscard]] constexpr size_type max_size() const noexcept;
    [[nodiscard]] constexpr size_type capacity() const noexcept;

    constexpr void clear() noexcept;
    constexpr void invalidate() noexcept;

    // you shall free memory returned by this method manually
    [[nodiscard]] constexpr T* release() noexcept;

    [[nodiscard]] constexpr _AllocationManager& getAllocationManager() noexcept;
    [[nodiscard]] constexpr const _AllocationManager& getAllocationManager() const noexcept;

    [[nodiscard]] constexpr Vector<T, _AllocationManager>& getVector() noexcept;
    [[nodiscard]] constexpr const Vector<T, _AllocationManager>& getVector() const noexcept;

    [[nodiscard]] constexpr size_type tell() const noexcept;
    constexpr Status seek(size_type offset) noexcept;

private:
    void setValidOffset(size_type offset) noexcept;

    Vector<T, _AllocationManager> m_vector;
    size_type m_offset{ 0 };
};

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Walker<T, _AllocationManager>::Walker(const Vector<T, _AllocationManager>& rhs)
{
    init(rhs);
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Walker<T, _AllocationManager>::Walker(Vector<T, _AllocationManager>&& rhs) noexcept
{
    init(std::move(rhs));
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::init(const Walker& rhs)
{
    if (this != &rhs)
    {
        CS_RUN(m_vector.init(rhs.m_vector));
        m_offset = rhs.m_offset;
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::init(Walker&& rhs) noexcept
{
    if (this != &rhs)
    {
        CS_RUN(m_vector.init(std::move(rhs.m_vector)));
        m_offset = rhs.m_offset;
        rhs.m_offset = 0;
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::init(const Vector<T, _AllocationManager>& rhs)
{
    if (&this->getVector() != &rhs)
    {
        CS_RUN(m_vector.init(rhs));
        m_offset = 0;
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::init(Vector<T, _AllocationManager>&& rhs) noexcept
{
    if (&this->getVector() != &rhs)
    {
        CS_RUN(m_vector.init(std::move(rhs)));
        m_offset = 0;
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::setSize(size_type n) noexcept
    requires std::is_trivially_copyable_v<T>
{
    Status status = m_vector.setSize(n);
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::reserve(size_type n)
{
    return m_vector.reserve(n);
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::reserve_from_current_offset(size_type n)
{
    if (m_offset + n < m_offset)
        return Status::ErrorOverflow;
    else
        return m_vector.reserve(m_offset + n);
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::pushBack(const T& value)
{
    Status status = m_vector.pushBack(value);
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::pushBack(T&& value)
{
    Status status = m_vector.pushBack(std::move(value));
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::pushBackN(const T* p, size_type n)
{
    Status status = m_vector.pushBackN(p, n);
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
template<typename V>
constexpr Status Walker<T, _AllocationManager>::pushBackArithmeticValue(V value)
    requires std::is_same_v<T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>)
{
    Status status = m_vector.pushBackArithmeticValue(value);
    m_offset = size();
    return status;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::replace(const T* p, size_type n, size_type offset)
{
    setValidOffset(offset);
    CS_RUN(m_vector.replace(p, n, offset));
    m_offset += n;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
template<typename V>
constexpr Status Walker<T, _AllocationManager>::readArithmeticValue(V& value) noexcept
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

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::insert(const T* p, size_type n, size_type offset)
{
    size_type oldSize = size();
    setValidOffset(offset);

    CS_RUN(m_vector.insert(p, n, offset));
    m_offset += size() - oldSize;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
template<typename ItSrc>
constexpr Status Walker<T, _AllocationManager>::insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin, iterator* pDestEnd)
{
    size_type oldSize = size();
    size_type newOffset = destBegin - m_vector.begin();
    setValidOffset(newOffset);

    CS_RUN(m_vector.insert(srcBegin, srcEnd, destBegin, pDestEnd));
    m_offset += size() - oldSize;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::erase(size_type offset, size_type n)
{
    setValidOffset(offset);
    return m_vector.erase(offset, n);
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::erase(iterator destBegin, iterator destEnd)
{
    size_type offset = destBegin - m_vector.begin();
    setValidOffset(offset);
    return m_vector.erase(destBegin, destEnd);
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::write(const T* p, size_type n)
{
    return replace(p, n, m_offset);
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::read(T* p, size_type n, size_type* pNRead)
{
    T* pNew = nullptr;

    CS_RUN(m_vector.copyN(m_offset, n, p, &pNew));
    
    size_type nReal = pNew - p;
    m_offset += nReal;

    if (pNRead)
        *pNRead = nReal;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr T* Walker<T, _AllocationManager>::data() noexcept
{
    return m_vector.data();
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr const T* Walker<T, _AllocationManager>::data() const noexcept
{
    return m_vector.data();
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr T& Walker<T, _AllocationManager>::operator[](size_type offset)
{ 
    setValidOffset(offset);
    return m_vector[offset];
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr const T& Walker<T, _AllocationManager>::operator[](size_type offset) const
{
    setValidOffset(offset);
    return m_vector[offset];
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr typename Walker<T, _AllocationManager>::size_type Walker<T, _AllocationManager>::size() const noexcept
{
    return m_vector.size();
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr typename Walker<T, _AllocationManager>::size_type Walker<T, _AllocationManager>::max_size() const noexcept
{
    return m_vector.max_size();
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr typename Walker<T, _AllocationManager>::size_type Walker<T, _AllocationManager>::capacity() const noexcept
{
    return m_vector.capacity();
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr void Walker<T, _AllocationManager>::clear() noexcept
{
    m_vector.clear(), m_offset = 0;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr void Walker<T, _AllocationManager>::invalidate() noexcept
{
    m_vector.invalidate(), m_offset = 0;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr T* Walker<T, _AllocationManager>::release() noexcept
{
    return m_offset = 0, m_vector.release();
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr _AllocationManager& Walker<T, _AllocationManager>::getAllocationManager() noexcept
{
    return m_vector.getAllocationManager();
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr const _AllocationManager& Walker<T, _AllocationManager>::getAllocationManager() const noexcept
{
    return m_vector.getAllocationManager();
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr Vector<T, _AllocationManager>& Walker<T, _AllocationManager>::getVector() noexcept
{
    return m_vector;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr const Vector<T, _AllocationManager>& Walker<T, _AllocationManager>::getVector() const noexcept
{
    return m_vector;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
[[nodiscard]] constexpr typename Walker<T, _AllocationManager>::size_type Walker<T, _AllocationManager>::tell() const noexcept
{
    return m_offset;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<T, _AllocationManager>::seek(size_type offset) noexcept
{
    setValidOffset(offset);
    return offset <= size() ? Status::NoError : Status::ErrorOverflow;
}

template<typename T, IAllocationManagerImpl _AllocationManager>
void Walker<T, _AllocationManager>::setValidOffset(size_type offset) noexcept
{
    m_offset = offset < size() ? offset : size();
}

} // namespace common_serialization
