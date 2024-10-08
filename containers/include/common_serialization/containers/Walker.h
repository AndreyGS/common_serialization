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

template<typename _T, IAllocationManagerImpl _AllocationManager = CtorStratAllocationManagerT<_T>>
class Walker
{
public:
    using value_type = typename Vector<_T, _AllocationManager>::value_type;
    using size_type = typename Vector<_T, _AllocationManager>::size_type;

    using iterator = VectorIterator<Vector<_T, _AllocationManager>>;
    using const_iterator = ConstVectorIterator<Vector<_T, _AllocationManager>>;

    using prefer_init_against_ctor = std::true_type;

    Walker() = default;

    explicit AGS_CS_ALWAYS_INLINE constexpr Walker(const Vector<_T, _AllocationManager>& rhs);
    explicit AGS_CS_ALWAYS_INLINE constexpr Walker(Vector<_T, _AllocationManager>&& rhs) noexcept;

    constexpr Status init(const Walker& rhs);
    constexpr Status init(Walker&& rhs) noexcept;
    constexpr Status init(const Vector<_T, _AllocationManager>& rhs);
    constexpr Status init(Vector<_T, _AllocationManager>&& rhs) noexcept;

    // only set data size, no default values are set
    constexpr Status setSize(size_type n) noexcept
        requires std::is_trivially_copyable_v<_T>;
    
    AGS_CS_ALWAYS_INLINE constexpr Status reserve(size_type n);
    constexpr Status reserve_from_current_offset(size_type n);

    constexpr Status pushBack(const _T& value);
    constexpr Status pushBack(_T&& value);
    constexpr Status pushBackN(const _T* p, size_type n);

    template<typename _V>
    constexpr Status pushBackArithmeticValue(_V value)
        requires std::is_same_v<_T, uint8_t> && (std::is_arithmetic_v<_V> || std::is_enum_v<_V>);

    constexpr Status replace(const _T* p, size_type n, size_type offset);

    constexpr Status insert(const _T* p, size_type n, size_type offset);
    template<typename ItSrc>
    constexpr Status insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin, iterator* pDestEnd = nullptr);

    constexpr Status erase(size_type offset, size_type n);
    constexpr Status erase(iterator destBegin, iterator destEnd);

    AGS_CS_ALWAYS_INLINE constexpr Status write(const _T* p, size_type n);
    
    // destination (p) must be initialized (for non pod-types)
    constexpr Status read(_T* p, size_type n, size_type* pNRead = nullptr);

    template<typename _V>
    constexpr Status readArithmeticValue(_V& value) noexcept
        requires std::is_same_v<_T, uint8_t> && (std::is_arithmetic_v<_V> || std::is_enum_v<_V>);

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] _T* data() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const _T* data() const noexcept;
    constexpr [[nodiscard]] _T& operator[](size_type offset);
    constexpr [[nodiscard]] const _T& operator[](size_type offset) const;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type size() const noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type max_size() const noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type capacity() const noexcept;

    AGS_CS_ALWAYS_INLINE constexpr void clear() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr void invalidate() noexcept;

    // you shall free memory returned by this method manually
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] _T* release() noexcept;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] _AllocationManager& getAllocationManager() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const _AllocationManager& getAllocationManager() const noexcept;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] Vector<_T, _AllocationManager>& getVector() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const Vector<_T, _AllocationManager>& getVector() const noexcept;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type tell() const noexcept;
    constexpr Status seek(size_type offset) noexcept;

private:
    void setValidOffset(size_type offset) noexcept;

    Vector<_T, _AllocationManager> m_vector;
    size_type m_offset{ 0 };
};

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Walker<_T, _AllocationManager>::Walker(const Vector<_T, _AllocationManager>& rhs)
{
    init(rhs);
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Walker<_T, _AllocationManager>::Walker(Vector<_T, _AllocationManager>&& rhs) noexcept
{
    init(std::move(rhs));
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::init(const Walker& rhs)
{
    if (this != &rhs)
    {
        AGS_CS_RUN(m_vector.init(rhs.m_vector));
        m_offset = rhs.m_offset;
    }

    return Status::NoError;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::init(Walker&& rhs) noexcept
{
    if (this != &rhs)
    {
        AGS_CS_RUN(m_vector.init(std::move(rhs.m_vector)));
        m_offset = rhs.m_offset;
        rhs.m_offset = 0;
    }

    return Status::NoError;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::init(const Vector<_T, _AllocationManager>& rhs)
{
    if (&this->getVector() != &rhs)
    {
        AGS_CS_RUN(m_vector.init(rhs));
        m_offset = 0;
    }

    return Status::NoError;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::init(Vector<_T, _AllocationManager>&& rhs) noexcept
{
    if (&this->getVector() != &rhs)
    {
        AGS_CS_RUN(m_vector.init(std::move(rhs)));
        m_offset = 0;
    }

    return Status::NoError;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::setSize(size_type n) noexcept
    requires std::is_trivially_copyable_v<_T>
{
    Status status = m_vector.setSize(n);
    m_offset = size();
    return status;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::reserve(size_type n)
{
    return m_vector.reserve(n);
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::reserve_from_current_offset(size_type n)
{
    if (m_offset + n < m_offset)
        return Status::ErrorOverflow;
    else
        return m_vector.reserve(m_offset + n);
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::pushBack(const _T& value)
{
    Status status = m_vector.pushBack(value);
    m_offset = size();
    return status;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::pushBack(_T&& value)
{
    Status status = m_vector.pushBack(std::move(value));
    m_offset = size();
    return status;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::pushBackN(const _T* p, size_type n)
{
    Status status = m_vector.pushBackN(p, n);
    m_offset = size();
    return status;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
template<typename _V>
constexpr Status Walker<_T, _AllocationManager>::pushBackArithmeticValue(_V value)
    requires std::is_same_v<_T, uint8_t> && (std::is_arithmetic_v<_V> || std::is_enum_v<_V>)
{
    Status status = m_vector.pushBackArithmeticValue(value);
    m_offset = size();
    return status;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::replace(const _T* p, size_type n, size_type offset)
{
    setValidOffset(offset);
    AGS_CS_RUN(m_vector.replace(p, n, offset));
    m_offset += n;

    return Status::NoError;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
template<typename _V>
constexpr Status Walker<_T, _AllocationManager>::readArithmeticValue(_V& value) noexcept
    requires std::is_same_v<_T, uint8_t> && (std::is_arithmetic_v<_V> || std::is_enum_v<_V>)
{
    if (sizeof(_V) + m_offset <= size())
    {
        value = *static_cast<const _V*>(static_cast<const void*>(data() + m_offset));
        m_offset += sizeof(_V);
        return Status::NoError;
    }
    else
        return Status::ErrorOverflow;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::insert(const _T* p, size_type n, size_type offset)
{
    size_type oldSize = size();
    setValidOffset(offset);

    AGS_CS_RUN(m_vector.insert(p, n, offset));
    m_offset += size() - oldSize;

    return Status::NoError;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
template<typename ItSrc>
constexpr Status Walker<_T, _AllocationManager>::insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin, iterator* pDestEnd)
{
    size_type oldSize = size();
    size_type newOffset = destBegin - m_vector.begin();
    setValidOffset(newOffset);

    AGS_CS_RUN(m_vector.insert(srcBegin, srcEnd, destBegin, pDestEnd));
    m_offset += size() - oldSize;

    return Status::NoError;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::erase(size_type offset, size_type n)
{
    setValidOffset(offset);
    return m_vector.erase(offset, n);
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::erase(iterator destBegin, iterator destEnd)
{
    size_type offset = destBegin - m_vector.begin();
    setValidOffset(offset);
    return m_vector.erase(destBegin, destEnd);
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::write(const _T* p, size_type n)
{
    return replace(p, n, m_offset);
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::read(_T* p, size_type n, size_type* pNRead)
{
    _T* pNew = nullptr;

    AGS_CS_RUN(m_vector.copyN(m_offset, n, p, &pNew));
    
    size_type nReal = pNew - p;
    m_offset += nReal;

    if (pNRead)
        *pNRead = nReal;

    return Status::NoError;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr _T* Walker<_T, _AllocationManager>::data() noexcept
{
    return m_vector.data();
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr const _T* Walker<_T, _AllocationManager>::data() const noexcept
{
    return m_vector.data();
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr _T& Walker<_T, _AllocationManager>::operator[](size_type offset)
{ 
    setValidOffset(offset);
    return m_vector[offset];
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr const _T& Walker<_T, _AllocationManager>::operator[](size_type offset) const
{
    setValidOffset(offset);
    return m_vector[offset];
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr typename Walker<_T, _AllocationManager>::size_type Walker<_T, _AllocationManager>::size() const noexcept
{
    return m_vector.size();
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr typename Walker<_T, _AllocationManager>::size_type Walker<_T, _AllocationManager>::max_size() const noexcept
{
    return m_vector.max_size();
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr typename Walker<_T, _AllocationManager>::size_type Walker<_T, _AllocationManager>::capacity() const noexcept
{
    return m_vector.capacity();
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr void Walker<_T, _AllocationManager>::clear() noexcept
{
    m_vector.clear(), m_offset = 0;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr void Walker<_T, _AllocationManager>::invalidate() noexcept
{
    m_vector.invalidate(), m_offset = 0;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr _T* Walker<_T, _AllocationManager>::release() noexcept
{
    return m_offset = 0, m_vector.release();
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr _AllocationManager& Walker<_T, _AllocationManager>::getAllocationManager() noexcept
{
    return m_vector.getAllocationManager();
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr const _AllocationManager& Walker<_T, _AllocationManager>::getAllocationManager() const noexcept
{
    return m_vector.getAllocationManager();
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Vector<_T, _AllocationManager>& Walker<_T, _AllocationManager>::getVector() noexcept
{
    return m_vector;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr const Vector<_T, _AllocationManager>& Walker<_T, _AllocationManager>::getVector() const noexcept
{
    return m_vector;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr typename Walker<_T, _AllocationManager>::size_type Walker<_T, _AllocationManager>::tell() const noexcept
{
    return m_offset;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
constexpr Status Walker<_T, _AllocationManager>::seek(size_type offset) noexcept
{
    setValidOffset(offset);
    return offset <= size() ? Status::NoError : Status::ErrorOverflow;
}

template<typename _T, IAllocationManagerImpl _AllocationManager>
void Walker<_T, _AllocationManager>::setValidOffset(size_type offset) noexcept
{
    m_offset = offset < size() ? offset : size();
}

} // namespace common_serialization
