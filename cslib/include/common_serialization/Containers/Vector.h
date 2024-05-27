/**
 * @file cslib/include/common_serialization/Containers/Vector.h
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

#include "common_serialization/Containers/IteratorTagsDeclarations.h"

namespace common_serialization
{

template<typename Vec>
class ConstVectorIterator
{
public:
    using iterator_concept = contiguous_iterator_tag;
    using iterator_category = random_access_iterator_tag;

    using value_type = typename Vec::value_type;
    using pointer = typename Vec::pointer;
    using const_pointer = typename Vec::const_pointer;
    using const_reference = typename Vec::const_reference;
    using difference_type = typename Vec::difference_type;

    explicit constexpr ConstVectorIterator(pointer p) : m_p(p) { }

    [[nodiscard]] constexpr bool operator==(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator!=(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator<(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator>=(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator>(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr bool operator<=(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr const_reference operator*() const;
    [[nodiscard]] constexpr const_pointer operator->() const;
    [[nodiscard]] constexpr const_reference operator[](difference_type n) const;
    constexpr ConstVectorIterator operator++() noexcept;
    constexpr ConstVectorIterator operator++(int) noexcept;
    constexpr ConstVectorIterator operator--() noexcept;
    constexpr ConstVectorIterator operator--(int) noexcept;
    constexpr ConstVectorIterator operator+=(difference_type n) noexcept;
    constexpr ConstVectorIterator operator-=(difference_type n) noexcept;
    
    [[nodiscard]] constexpr ConstVectorIterator operator-(difference_type n) const noexcept;
    [[nodiscard]] constexpr difference_type operator-(ConstVectorIterator it) const noexcept;

    [[nodiscard]] constexpr const_pointer getPointer() const noexcept;

    [[nodiscard]] friend constexpr ConstVectorIterator operator+(ConstVectorIterator it, difference_type n) noexcept
    {
        return it.m_p + n;
    }

protected:
    pointer m_p{ nullptr };
};

template<typename Vec>
[[nodiscard]] constexpr bool ConstVectorIterator<Vec>::operator==(const ConstVectorIterator& rhs) const noexcept
{
    return m_p == rhs.m_p;
}

template<typename Vec>
[[nodiscard]] constexpr bool ConstVectorIterator<Vec>::operator!=(const ConstVectorIterator& rhs) const noexcept
{
    return !operator==(rhs);
}

template<typename Vec>
[[nodiscard]] constexpr bool ConstVectorIterator<Vec>::operator<(const ConstVectorIterator& rhs) const noexcept
{
    return rhs.m_p > m_p;
}

template<typename Vec>
[[nodiscard]] constexpr bool ConstVectorIterator<Vec>::operator>=(const ConstVectorIterator& rhs) const noexcept
{
    return !operator<(rhs);
}

template<typename Vec>
[[nodiscard]] constexpr bool ConstVectorIterator<Vec>::operator>(const ConstVectorIterator& rhs) const noexcept
{
    return rhs.m_p < m_p;
}

template<typename Vec>
[[nodiscard]] constexpr bool ConstVectorIterator<Vec>::operator<=(const ConstVectorIterator& rhs) const noexcept
{
    return !operator>(rhs);
}

template<typename Vec>
[[nodiscard]] constexpr typename ConstVectorIterator<Vec>::const_reference ConstVectorIterator<Vec>::operator*() const
{
    return *m_p;
}

template<typename Vec>
[[nodiscard]] constexpr typename ConstVectorIterator<Vec>::const_pointer ConstVectorIterator<Vec>::operator->() const
{
    return m_p;
}

template<typename Vec>
[[nodiscard]] constexpr typename ConstVectorIterator<Vec>::const_reference ConstVectorIterator<Vec>::operator[](difference_type n) const
{
    return *(m_p + n);
}

template<typename Vec>
constexpr ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator++() noexcept
{
    return ++m_p, * this;
}

template<typename Vec>
constexpr ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator++(int) noexcept
{
    return ConstVectorIterator(m_p++);
}

template<typename Vec>
constexpr ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator--() noexcept
{
    return --m_p, * this;
}

template<typename Vec>
constexpr ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator--(int) noexcept
{
    return ConstVectorIterator(m_p--);
}

template<typename Vec>
constexpr ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator+=(difference_type n) noexcept
{
    return m_p += n, *this;
}

template<typename Vec>
constexpr ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator-=(difference_type n) noexcept
{
    return m_p -= n, *this;
}

template<typename Vec>
[[nodiscard]] constexpr ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator-(difference_type n) const noexcept
{
    return ConstVectorIterator(m_p - n);
}

template<typename Vec>
[[nodiscard]] constexpr typename ConstVectorIterator<Vec>::difference_type ConstVectorIterator<Vec>::operator-(ConstVectorIterator it) const noexcept
{
    return m_p - it.m_p;
}

template<typename Vec>
[[nodiscard]] constexpr typename ConstVectorIterator<Vec>::const_pointer ConstVectorIterator<Vec>::getPointer() const noexcept
{
    return m_p;
}

template<typename Vec>
class VectorIterator : public ConstVectorIterator<Vec>
{
    using Base = ConstVectorIterator<Vec>;
public:
    using iterator_concept = contiguous_iterator_tag;
    using iterator_category = random_access_iterator_tag;

    using value_type = typename Vec::value_type;
    using pointer = typename Vec::pointer;   
    using reference = typename Vec::reference;
    using difference_type = typename Vec::difference_type;

    explicit constexpr VectorIterator(pointer p) : Base(p) { }

    [[nodiscard]] constexpr reference operator*() const;
    [[nodiscard]] constexpr pointer operator->() const;
    [[nodiscard]] constexpr reference operator[](difference_type n) const;
    constexpr VectorIterator operator++() noexcept;
    constexpr VectorIterator operator++(int) noexcept;
    constexpr VectorIterator operator--() noexcept;
    constexpr VectorIterator operator--(int) noexcept;
    constexpr VectorIterator operator+=(difference_type n) noexcept;
    constexpr VectorIterator operator-=(difference_type n) noexcept;

    constexpr difference_type operator-(VectorIterator n) const noexcept;
    [[nodiscard]] constexpr VectorIterator operator-(difference_type n) const noexcept;

    [[nodiscard]] constexpr pointer getPointer() noexcept;

    [[nodiscard]] friend constexpr VectorIterator operator+(VectorIterator it, difference_type n) noexcept
    {
        return VectorIterator(it.m_p + n);
    }
};

template<typename Vec>
[[nodiscard]] constexpr typename VectorIterator<Vec>::reference VectorIterator<Vec>::operator*() const
{
    return const_cast<reference>(Base::operator*());
}

template<typename Vec>
[[nodiscard]] constexpr typename VectorIterator<Vec>::pointer VectorIterator<Vec>::operator->() const
{
    return const_cast<pointer>(Base::operator->());
}

template<typename Vec>
[[nodiscard]] constexpr typename VectorIterator<Vec>::reference VectorIterator<Vec>::operator[](difference_type n) const
{
    return const_cast<reference>(Base::operator[](n));
}

template<typename Vec>
constexpr VectorIterator<Vec> VectorIterator<Vec>::operator++() noexcept
{
    return ++this->m_p, * this;
}

template<typename Vec>
constexpr VectorIterator<Vec> VectorIterator<Vec>::operator++(int) noexcept
{
    return VectorIterator(this->m_p++);
}

template<typename Vec>
constexpr VectorIterator<Vec> VectorIterator<Vec>::operator--() noexcept
{
    return --this->m_p, *this;
}

template<typename Vec>
constexpr VectorIterator<Vec> VectorIterator<Vec>::operator--(int) noexcept
{
    return VectorIterator(this->m_p--);
}

template<typename Vec>
constexpr VectorIterator<Vec> VectorIterator<Vec>::operator+=(difference_type n) noexcept
{
    return this->m_p += n, *this;
}

template<typename Vec>
constexpr VectorIterator<Vec> VectorIterator<Vec>::operator-=(difference_type n) noexcept
{
    return this->m_p -= n, *this;
}

template<typename Vec>
[[nodiscard]] constexpr VectorIterator<Vec> VectorIterator<Vec>::operator-(difference_type n) const noexcept
{
    return VectorIterator(this->m_p - n);
}

template<typename Vec>
[[nodiscard]] constexpr typename VectorIterator<Vec>::difference_type VectorIterator<Vec>::operator-(VectorIterator it) const noexcept
{
    return this->m_p - it.m_p;
}

template<typename Vec>
[[nodiscard]] constexpr typename VectorIterator<Vec>::pointer VectorIterator<Vec>::getPointer() noexcept
{
    return this->m_p;
}

/// @brief Container of elements stored contiguously
/// @tparam _T Type of elements
/// @tparam AllocatorHelper Allocator Helper using for storage management
///     and objects creation/deletion
template<typename _T, typename AllocatorHelper = CStrategicAllocatorHelperT<_T>>
class Vector
{
public:
    static_assert(std::is_same_v<_T, typename AllocatorHelper::value_type>, "Types _T and AllocatorHelper::value_type are not the same");

    using value_type = _T;
    using size_type = typename AllocatorHelper::size_type;
    using difference_type = typename AllocatorHelper::difference_type;
    using constructor_allocator = typename AllocatorHelper::constructor_allocator;

    using pointer = _T*;
    using const_pointer = const _T*;
    using reference = _T&;
    using const_reference = const _T&;

    using iterator = VectorIterator<Vector<value_type, AllocatorHelper>>;
    using const_iterator = ConstVectorIterator<Vector<value_type, AllocatorHelper>>;

    constexpr Vector() = default;
    constexpr Vector(const Vector& rhs);
    constexpr Vector(Vector&& rhs) noexcept;
    constexpr Vector& operator=(const Vector& rhs);
    constexpr Vector& operator=(Vector&& rhs) noexcept;
    constexpr ~Vector() noexcept;

    /// @brief Copy init method
    /// @param rhs Another instance
    /// @return Status of initialization
    constexpr Status init(const Vector& rhs);

    /// @brief Move init method
    /// @param rhs Another instance
    /// @return Status of initialization
    constexpr Status init(Vector&& rhs) noexcept;

    // only set data size, no default values are set

    /// @brief Set size of data
    /// @note For trivially copyable types.
    ///     Only set data size, no default values are set.
    /// @param n New size
    /// @return Status of operation
    constexpr Status setSize(size_type n) noexcept
        requires std::is_trivially_copyable_v<_T>;

    /// @brief Preallocate at least that much memory that
    ///     is enough to hold N elements of _T
    /// @param n Number of elements that underlying storage
    ///     must be capable to hold
    /// @return Status of operation
    constexpr Status reserve(size_type n);
    
    /// @brief Append element to tail of the storage
    /// @param value Value that need to append
    /// @return Status of operation
    constexpr Status pushBack(const _T& value);

    /// @brief Append element to tail of the storage
    /// @param value Value that need to move to storage
    /// @return Status of operation
    constexpr Status pushBack(_T&& value);

    /// @brief Append N elements to tail of the storage
    /// @param p Pointer to array of elements
    /// @param n Number of elements that need to append
    /// @return Status of operation
    constexpr Status pushBackN(const _T* p, size_type n);

    /// @brief Append arithmetic or enum value to tail of container
    /// @remark Using for eliminating redundant overhead on raw arrays
    /// @tparam V Type of value that
    /// @param value Value that need to append
    /// @return Status of operation
    template<typename V>
    constexpr Status pushBackArithmeticValue(V value)
        requires std::is_same_v<_T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>);

    template<typename... Ts>
    constexpr Status emplaceBack(Ts&&... ts);

    /// @brief Replace N elements from offset
    /// @note Overwrites existing elements if offset < size.
    ///     If offset + N > size, size of container will be increased.
    ///     If offset > size an error will returned. It is
    ///     legal to pass p == nullptr if n == 0 as well. 
    /// @param p Pointer to array of elements
    /// @param n Number of elements that need to copy
    /// @param offset Offset from which replace is started
    /// @param pNewOffset Offset + N if there is no error
    /// @return Status of operation
    constexpr Status replace(const _T* p, size_type n, size_type offset, size_type* pNewOffset = nullptr);

    /// @brief Insert N elements from offset
    /// @note  
    /// @param p 
    /// @param n 
    /// @param offset 
    /// @param pNewOffset 
    /// @return 
    constexpr Status insert(const _T* p, size_type n, size_type offset, size_type* pNewOffset = nullptr);
    constexpr Status insert(const _T& value, size_type offset);
    template<typename ItSrc>
    constexpr Status insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin, iterator* pDestEnd = nullptr);

    constexpr Status erase(size_type offset);
    constexpr Status erase(size_type offset, size_type n);
    constexpr Status erase(iterator pos);
    constexpr Status erase(iterator destBegin, iterator destEnd);

    // copy from Vector to p (destination must be initialized (for non pod-types))
    constexpr Status copyN(size_type offset, size_type n, _T* p, _T** ppNew = nullptr);

    // copy from Vector to destBegin... (destination must be initialized (for non pod-types))
    template<typename ItDest>
    constexpr Status copyN(iterator srcBegin, iterator srcEnd, ItDest destBegin, ItDest* pDestEnd = nullptr);

    [[nodiscard]] constexpr _T* data() noexcept;
    [[nodiscard]] constexpr const _T* data() const noexcept;

    [[nodiscard]] constexpr _T& operator[](size_type offset);
    [[nodiscard]] constexpr const _T& operator[](size_type offset) const;
    
    [[nodiscard]] constexpr _T& back();
    [[nodiscard]] constexpr const _T& back() const;

    [[nodiscard]] constexpr _T& front();
    [[nodiscard]] constexpr const _T& front() const;

    [[nodiscard]] constexpr size_type size() const noexcept;
    [[nodiscard]] constexpr size_type max_size() const noexcept;
    [[nodiscard]] constexpr size_type capacity() const noexcept;

    constexpr void clear() noexcept;
    constexpr void invalidate() noexcept;

    // you shall free memory returned by this method manually
    [[nodiscard]] constexpr _T* release() noexcept;

    [[nodiscard]] constexpr iterator begin() noexcept;
    [[nodiscard]] constexpr const_iterator begin() const noexcept;
    [[nodiscard]] constexpr iterator end() noexcept;
    [[nodiscard]] constexpr const_iterator end() const noexcept;
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept;
    [[nodiscard]] constexpr const_iterator cend() const noexcept;

    [[nodiscard]] constexpr AllocatorHelper& getAllocatorHelper() noexcept;
    [[nodiscard]] constexpr const AllocatorHelper& getAllocatorHelper() const noexcept;

    [[nodiscard]] constexpr bool operator==(const Vector& rhs) const
        requires (IsNotPointer<_T> || IsNotPointer<std::remove_pointer_t<_T>>);

private:
    [[nodiscard]] constexpr Status reserveInternal(size_type n, bool strict);
    [[nodiscard]] constexpr Status addSpaceIfNeed(size_type n);
    [[nodiscard]] constexpr bool isIteratorNotDereferenceable(iterator it) const noexcept;

    _T* m_p{ nullptr };
    size_type m_dataSize{ 0 };
    size_type m_allocatedSize{ 0 };

    AllocatorHelper m_allocatorHelper;

private:
    // It's not mandatory to have friend csp::processing::deserialize function 
    // to have deserialization capability.
    // The only reason this is done is because it allows some optimizations in
    // processing when we have direct access to private fields.
    template<typename C, typename A, typename X>
    friend Status csp::processing::data::templates::deserialize(X& ctx, Vector<C, A>& value);
};

template<typename _T, typename AllocatorHelper>
constexpr Vector<_T, AllocatorHelper>::Vector(const Vector& rhs)
{
    init(rhs);
}

template<typename _T, typename AllocatorHelper>
constexpr Vector<_T, AllocatorHelper>::Vector(Vector&& rhs) noexcept
{
    init(std::move(rhs));
}

template<typename _T, typename AllocatorHelper>
constexpr Vector<_T, AllocatorHelper>& Vector<_T, AllocatorHelper>::operator=(const Vector& rhs)
{
    init(rhs);

    return *this;
}

template<typename _T, typename AllocatorHelper>
constexpr Vector<_T, AllocatorHelper>& Vector<_T, AllocatorHelper>::operator=(Vector&& rhs) noexcept
{
    init(std::move(rhs));

    return *this;
}

template<typename _T, typename AllocatorHelper>
constexpr Vector<_T, AllocatorHelper>::~Vector() noexcept
{
    invalidate();
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::init(const Vector& rhs)
{
    if (this != &rhs)
    {
        clear();

        m_allocatorHelper = rhs.m_allocatorHelper;

        if (rhs.m_dataSize > m_allocatedSize)
        {
            m_allocatorHelper.deallocate(m_p);
            m_dataSize = 0;
            m_allocatedSize = 0;

            m_p = m_allocatorHelper.allocate(rhs.m_dataSize, &m_allocatedSize);

            if (!m_p)
                return Status::ErrorNoMemory;
        }

        CS_RUN(m_allocatorHelper.copyNoOverlap(m_p, rhs.m_p, rhs.m_dataSize));
        m_dataSize = rhs.m_dataSize;
    }

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::init(Vector&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_allocatorHelper.destroyAndDeallocate(m_p, m_dataSize);

        m_dataSize = rhs.m_dataSize;
        m_allocatedSize = rhs.m_allocatedSize;
        m_allocatorHelper = std::move(rhs.m_allocatorHelper);

        m_p = rhs.release();
    }

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::setSize(size_type n) noexcept
    requires std::is_trivially_copyable_v<_T>
{
    CS_RUN(reserveInternal(n, false));
    m_dataSize = n;
    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::reserve(size_type n)
{
    return reserveInternal(n, true);
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::pushBack(const _T& value)
{
    CS_RUN(addSpaceIfNeed(1));
    CS_RUN(m_allocatorHelper.construct(m_p + m_dataSize, value));
    ++m_dataSize;

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::pushBack(_T&& value)
{
    CS_RUN(addSpaceIfNeed(1));
    CS_RUN(m_allocatorHelper.moveNoOverlap(m_p + m_dataSize, &value, 1));
    ++m_dataSize;

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::pushBackN(const _T* p, size_type n)
{
    if (p == nullptr && n != 0)
        return Status::ErrorInvalidArgument;

    CS_RUN(addSpaceIfNeed(n));

    CS_RUN(m_allocatorHelper.copyNoOverlap(m_p + m_dataSize, p, n));
    m_dataSize += n;

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
template<typename V>
constexpr Status Vector<_T, AllocatorHelper>::pushBackArithmeticValue(V value)
    requires std::is_same_v<_T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>)
{
    Status status = Status::NoError;

    CS_RUN(addSpaceIfNeed(sizeof(V)));

    *static_cast<V*>(static_cast<void*>(m_p + m_dataSize)) = value;
    m_dataSize += sizeof(V);

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
template<typename... Ts>
constexpr Status Vector<_T, AllocatorHelper>::emplaceBack(Ts&&... ts)
{
    Status status = Status::NoError;

    CS_RUN(addSpaceIfNeed(1));
    CS_RUN(m_allocatorHelper.construct(m_p + m_dataSize, std::forward<Ts>(ts)...));
    ++m_dataSize;

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::replace(const _T* p, size_type n, size_type offset, size_type* pNewOffset)
{
    if (p == nullptr && n != 0)
        return Status::ErrorInvalidArgument;
    
    // don't allow to create sparse array by this function
    if (offset > m_dataSize)
        return Status::ErrorOverflow;

    size_type offsetPlusN = offset + n;
    size_type newDataSize = offsetPlusN > m_dataSize ? offsetPlusN : m_dataSize;

    // if newDataSize <= m_allocatedSize there is no need to allocate memory or there is overflow
    if (newDataSize > m_allocatedSize)
    {
        size_type newAllocatedSize = 0;
        pointer newMp = m_allocatorHelper.allocate(newDataSize, &newAllocatedSize);
        if (newMp)
        {
            m_allocatedSize = newAllocatedSize;
            CS_RUN(m_allocatorHelper.moveNoOverlap(newMp, m_p, offset));
            CS_RUN(m_allocatorHelper.copyNoOverlap(newMp + offset, p, n));
            m_allocatorHelper.destroyAndDeallocate(m_p, m_dataSize);

            m_p = newMp;
        }
        else
            return Status::ErrorNoMemory;
    }
    else if (newDataSize >= m_dataSize) // there is no overflow
    {
        if constexpr (constructor_allocator::value)
            m_allocatorHelper.destroyN(m_p + offset, n >= m_dataSize - offset ? m_dataSize - offset : n);

        CS_RUN(m_allocatorHelper.copyNoOverlap(m_p + offset, p, n));
    }

    m_dataSize = newDataSize;

    if (pNewOffset)
        *pNewOffset = offsetPlusN;

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::insert(const _T* p, size_type n, size_type offset, size_type* pNewOffset)
{
    if (p == nullptr && n != 0)
        return Status::ErrorInvalidArgument;

    // don't allow to create sparse array by this function
    if (offset > m_dataSize)
        return Status::ErrorOverflow;

    size_type offsetPlusN = offset + n;
    size_type newDataSize = m_dataSize + n;
    size_type rightDataPartSize = m_dataSize - offset;

    // if newDataSize <= m_allocatedSize there is no need to allocate memory or there is overflow
    if (newDataSize > m_allocatedSize)
    {
        size_type newAllocatedSize = 0;
        pointer newMp = m_allocatorHelper.allocate(newDataSize, &newAllocatedSize);
        if (newMp)
        {
            m_allocatedSize = newAllocatedSize;
            CS_RUN(m_allocatorHelper.moveNoOverlap(newMp, m_p, offset));
            CS_RUN(m_allocatorHelper.copyNoOverlap(newMp + offset, p, n));

            if (offset != m_dataSize) // if we do not push back
                CS_RUN(m_allocatorHelper.moveNoOverlap(newMp + offsetPlusN, m_p + offset, rightDataPartSize));

            m_allocatorHelper.destroyAndDeallocate(m_p, m_dataSize);

            m_p = newMp;
        }
        else
            return Status::ErrorNoMemory;
    }
    else if (newDataSize > m_dataSize)
    {
        if (offset != m_dataSize)
            CS_RUN(m_allocatorHelper.moveDirty(m_p + offsetPlusN, m_p + m_dataSize, m_p + offset, rightDataPartSize));

        CS_RUN(m_allocatorHelper.copyNoOverlap(m_p + offset, p, n));
    }
    else if (newDataSize < m_dataSize)
        return Status::ErrorOverflow;

    m_dataSize = newDataSize;

    if (pNewOffset)
        *pNewOffset = offsetPlusN;

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::insert(const _T& value, size_type offset)
{
    return insert(&value, 1, offset);
}

template<typename _T, typename AllocatorHelper>
template<typename ItSrc>
constexpr Status Vector<_T, AllocatorHelper>::insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin, iterator* pDestEnd)
{
    if (isIteratorNotDereferenceable(destBegin) && destBegin != end())
        return Status::ErrorOverflow;

    Vector<_T, AllocatorHelper> temp;
    size_type oldDataSize = m_dataSize;
    size_type currentOffset = destBegin.getPointer() - m_p;

    while (srcBegin != srcEnd)
    {
        if (currentOffset < m_allocatedSize)
        {
            pointer pCurrent = m_p + currentOffset;
            if (m_dataSize > currentOffset)
            {
                CS_RUN(temp.pushBack(std::move(*pCurrent)));
                m_allocatorHelper.destroy(pCurrent); // if _T is not moveable we should destroying its objects explicitly
                ++currentOffset;
            }
            else
                m_dataSize = ++currentOffset;

            CS_RUN(m_allocatorHelper.copyNoOverlap(pCurrent, &*srcBegin++, 1));
        }
        else
            CS_RUN(reserveInternal(currentOffset + 1, false));
    }

    if (temp.size())
    {
        if (oldDataSize > currentOffset)
        {
            if constexpr (constructor_allocator::value)
                for (size_type i = currentOffset; i < oldDataSize; ++i)
                {
                    CS_RUN(temp.pushBack(std::move(*(m_p + i))));
                    m_allocatorHelper.destroy(m_p + i); // if _T is not moveable we should destroying its objects explicitly
                }
            else
                CS_RUN(temp.pushBackN(m_p + currentOffset, oldDataSize - currentOffset));
        }

        m_dataSize = currentOffset;
        CS_RUN(addSpaceIfNeed(temp.size()));

        CS_RUN(m_allocatorHelper.moveNoOverlap(m_p + currentOffset, temp.data(), temp.size()));
        m_dataSize = currentOffset + temp.size();
    }

    if (pDestEnd)
        *pDestEnd = iterator(m_p + currentOffset);

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::erase(size_type offset)
{
    return erase(offset, 1);
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::erase(size_type offset, size_type n)
{
    if (offset > m_dataSize || offset == m_dataSize && n != 0)
        return Status::ErrorOverflow;
    else if (n == 0)
        return Status::NoError;

    if (offset + n > m_dataSize || offset + n < offset)
        n = m_dataSize - offset;

    m_allocatorHelper.destroyN(m_p + offset, n);

    difference_type rightNStart = offset + n;
    difference_type rightN = m_dataSize - rightNStart;

    CS_RUN(m_allocatorHelper.move(m_p + offset, m_p + rightNStart, rightN));
    m_allocatorHelper.destroyN(m_p + rightNStart, rightN); // if _T is not moveable we should destroying its objects explicitly

    m_dataSize = offset + rightN;

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::erase(iterator pos)
{
    return erase(pos, pos + 1);
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::erase(iterator destBegin, iterator destEnd)
{
    if (destBegin > destEnd)
        return Status::ErrorOverflow;
    else if (isIteratorNotDereferenceable(destBegin))
        return Status::ErrorInvalidArgument;
    else if (destBegin == destEnd)
        return Status::NoError;

    if (destEnd > end())
        destEnd = end();

    difference_type n = destEnd - destBegin;

    m_allocatorHelper.destroyN(destBegin.getPointer(), n);

    difference_type leftN = destBegin - begin();
    difference_type rightN = m_dataSize - n - leftN;

    CS_RUN(m_allocatorHelper.move(destBegin.getPointer(), destEnd.getPointer(), rightN));
    m_allocatorHelper.destroyN(destEnd.getPointer(), rightN); // if _T is not moveable we should destroying its objects explicitly

    m_dataSize = leftN + rightN;

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
constexpr Status Vector<_T, AllocatorHelper>::copyN(size_type offset, size_type n, _T* p, _T** ppNew)
{
    if (p == nullptr && n != 0)
        return Status::ErrorInvalidArgument;

    if (offset > m_dataSize || offset == m_dataSize && n != 0)
        return Status::ErrorOverflow;

    difference_type nReal = offset + n > m_dataSize ? m_dataSize - offset : n;

    CS_RUN(m_allocatorHelper.copyDirty(p, p + offset, m_p + offset, nReal));

    if (ppNew)
        *ppNew = p + nReal;

    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
template<typename ItDest>
constexpr Status Vector<_T, AllocatorHelper>::copyN(iterator srcBegin, iterator srcEnd, ItDest destBegin, ItDest* pDestEnd)
{
    if (isIteratorNotDereferenceable(srcBegin))
        return Status::ErrorInvalidArgument;

    if (srcBegin > srcEnd)
        return Status::ErrorOverflow;

    if (srcEnd > end())
        srcEnd = end();

    while (srcBegin != srcEnd)
    {
        CS_RUN(m_allocatorHelper.copyDirty(&*destBegin, &*destBegin + 1, srcBegin++.getPointer(), 1));
        ++destBegin;
    }

    if (pDestEnd)
        *pDestEnd = destBegin;
    
    return Status::NoError;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr _T* Vector<_T, AllocatorHelper>::data() noexcept
{
    return m_p;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr const _T* Vector<_T, AllocatorHelper>::data() const noexcept
{
    return m_p;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr _T& Vector<_T, AllocatorHelper>::operator[](size_type offset)
{
    return *(m_p + offset);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr const _T& Vector<_T, AllocatorHelper>::operator[](size_type offset) const
{
    return *(m_p + offset);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr _T& Vector<_T, AllocatorHelper>::back()
{
    return *(m_p + size() - 1);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr const _T& Vector<_T, AllocatorHelper>::back() const
{
    return *(m_p + size() - 1);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr _T& Vector<_T, AllocatorHelper>::front()
{
    return *m_p;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr const _T& Vector<_T, AllocatorHelper>::front() const
{
    return *m_p;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr typename Vector<_T, AllocatorHelper>::size_type Vector<_T, AllocatorHelper>::size() const noexcept
{
    return m_dataSize;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr typename Vector<_T, AllocatorHelper>::size_type Vector<_T, AllocatorHelper>::max_size() const noexcept
{
    return m_allocatorHelper.max_size();
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr typename Vector<_T, AllocatorHelper>::size_type Vector<_T, AllocatorHelper>::capacity() const noexcept
{
    return m_allocatedSize;
}

template<typename _T, typename AllocatorHelper>
constexpr void Vector<_T, AllocatorHelper>::clear() noexcept
{
    m_allocatorHelper.destroyN(m_p, m_dataSize);
    m_dataSize = 0;
}

template<typename _T, typename AllocatorHelper>
constexpr void Vector<_T, AllocatorHelper>::invalidate() noexcept
{
    clear();
    m_allocatorHelper.deallocate(m_p);
    m_p = nullptr;
    m_allocatedSize = 0;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr _T* Vector<_T, AllocatorHelper>::release() noexcept
{
    _T* s = m_p;
    m_p = 0;
    m_dataSize = 0;
    m_allocatedSize = 0;
    return s;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr typename Vector<_T, AllocatorHelper>::iterator Vector<_T, AllocatorHelper>::begin() noexcept
{
    return iterator(m_p);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr typename Vector<_T, AllocatorHelper>::const_iterator Vector<_T, AllocatorHelper>::begin() const noexcept
{
    return const_iterator(m_p);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr typename Vector<_T, AllocatorHelper>::iterator Vector<_T, AllocatorHelper>::end() noexcept
{
    return iterator(m_p + m_dataSize);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr typename Vector<_T, AllocatorHelper>::const_iterator Vector<_T, AllocatorHelper>::end() const noexcept
{
    return const_iterator(m_p + m_dataSize);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr typename Vector<_T, AllocatorHelper>::const_iterator Vector<_T, AllocatorHelper>::cbegin() const noexcept
{
    return const_iterator(m_p);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr typename Vector<_T, AllocatorHelper>::const_iterator Vector<_T, AllocatorHelper>::cend() const noexcept
{
    return const_iterator(m_p + m_dataSize);
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr AllocatorHelper& Vector<_T, AllocatorHelper>::getAllocatorHelper() noexcept
{
    return m_allocatorHelper;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr const AllocatorHelper& Vector<_T, AllocatorHelper>::getAllocatorHelper() const noexcept
{
    return m_allocatorHelper;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr Status Vector<_T, AllocatorHelper>::reserveInternal(size_type n, bool strict)
{
    if (n > m_allocatedSize)
    {
        _T* pNewMp = strict ? m_allocatorHelper.allocateStrict(n) : m_allocatorHelper.allocate(n, &n);
        if (pNewMp)
        {
            m_allocatedSize = n;

            if (Status status = m_allocatorHelper.moveNoOverlap(pNewMp, m_p, m_dataSize); !statusSuccess(status))
            {
                m_dataSize = 0;
                m_allocatorHelper.deallocate(pNewMp);
                return status;
            }

            m_allocatorHelper.destroyAndDeallocate(m_p, m_dataSize);
            m_p = pNewMp;
        }
        else
            return Status::ErrorNoMemory;
    }

    return Status::NoError;
}


template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr Status Vector<_T, AllocatorHelper>::addSpaceIfNeed(size_type n)
{
    return m_dataSize + n >= m_dataSize
        ? m_dataSize + n > m_allocatedSize
            ? reserveInternal(m_dataSize + n, false)
            : Status::NoError
        : Status::ErrorOverflow;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr bool Vector<_T, AllocatorHelper>::isIteratorNotDereferenceable(iterator it) const noexcept
{
    return &*it < m_p || &*it >= m_p + m_dataSize;
}

template<typename _T, typename AllocatorHelper>
[[nodiscard]] constexpr bool Vector<_T, AllocatorHelper>::operator==(const Vector& rhs) const
    requires (IsNotPointer<_T> || IsNotPointer<std::remove_pointer_t<_T>>)
{
    if (size() != rhs.size())
        return false;

    for (size_type i = 0; i < size(); ++i)
        if constexpr (IsNotPointer<_T>)
        {
            if (m_p[i] != rhs.m_p[i])
                return false;
        }
        else // if constexpr (IsNotPointer<std::remove_pointer_t<_T>>)
        {
            if (m_p[i] == nullptr || rhs.m_p[i] == nullptr)
                return m_p[i] == rhs.m_p[i];
            
            if (*m_p[i] != *rhs.m_p[i])
                return false;
        }

    return true;
}

} // namespace common_serialization
