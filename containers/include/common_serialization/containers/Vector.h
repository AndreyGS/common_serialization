/**
 * @file common_serialization/containers/Vector.h
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

#include <common_serialization/allocation_manager_interfaces/IAllocationManager.h>
#include <common_serialization/containers/containers_config.h>
#include <common_serialization/containers/IteratorTagsDeclarations.h>

namespace common_serialization::csp::processing::data
{

template<typename T, typename... Ts>
class TemplateProcessor;

}

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

    constexpr [[nodiscard]] bool operator==(const ConstVectorIterator& rhs) const noexcept;
    constexpr [[nodiscard]] bool operator!=(const ConstVectorIterator& rhs) const noexcept;
    constexpr [[nodiscard]] bool operator<(const ConstVectorIterator& rhs) const noexcept;
    constexpr [[nodiscard]] bool operator>=(const ConstVectorIterator& rhs) const noexcept;
    constexpr [[nodiscard]] bool operator>(const ConstVectorIterator& rhs) const noexcept;
    constexpr [[nodiscard]] bool operator<=(const ConstVectorIterator& rhs) const noexcept;
    constexpr [[nodiscard]] const_reference operator*() const;
    constexpr [[nodiscard]] const_pointer operator->() const;
    constexpr [[nodiscard]] const_reference operator[](difference_type n) const;
    constexpr ConstVectorIterator operator++() noexcept;
    constexpr ConstVectorIterator operator++(int) noexcept;
    constexpr ConstVectorIterator operator--() noexcept;
    constexpr ConstVectorIterator operator--(int) noexcept;
    constexpr ConstVectorIterator operator+=(difference_type n) noexcept;
    constexpr ConstVectorIterator operator-=(difference_type n) noexcept;
    
    constexpr [[nodiscard]] ConstVectorIterator operator-(difference_type n) const noexcept;
    constexpr [[nodiscard]] difference_type operator-(ConstVectorIterator it) const noexcept;

    constexpr [[nodiscard]] const_pointer getPointer() const noexcept;

    friend constexpr [[nodiscard]] ConstVectorIterator operator+(ConstVectorIterator it, difference_type n) noexcept
    {
        return it.m_p + n;
    }

protected:
    pointer m_p{ nullptr };
};

template<typename Vec>
constexpr bool ConstVectorIterator<Vec>::operator==(const ConstVectorIterator& rhs) const noexcept
{
    return m_p == rhs.m_p;
}

template<typename Vec>
constexpr bool ConstVectorIterator<Vec>::operator!=(const ConstVectorIterator& rhs) const noexcept
{
    return !operator==(rhs);
}

template<typename Vec>
constexpr bool ConstVectorIterator<Vec>::operator<(const ConstVectorIterator& rhs) const noexcept
{
    return rhs.m_p > m_p;
}

template<typename Vec>
constexpr bool ConstVectorIterator<Vec>::operator>=(const ConstVectorIterator& rhs) const noexcept
{
    return !operator<(rhs);
}

template<typename Vec>
constexpr bool ConstVectorIterator<Vec>::operator>(const ConstVectorIterator& rhs) const noexcept
{
    return rhs.m_p < m_p;
}

template<typename Vec>
constexpr bool ConstVectorIterator<Vec>::operator<=(const ConstVectorIterator& rhs) const noexcept
{
    return !operator>(rhs);
}

template<typename Vec>
constexpr typename ConstVectorIterator<Vec>::const_reference ConstVectorIterator<Vec>::operator*() const
{
    return *m_p;
}

template<typename Vec>
constexpr typename ConstVectorIterator<Vec>::const_pointer ConstVectorIterator<Vec>::operator->() const
{
    return m_p;
}

template<typename Vec>
constexpr typename ConstVectorIterator<Vec>::const_reference ConstVectorIterator<Vec>::operator[](difference_type n) const
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
constexpr ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator-(difference_type n) const noexcept
{
    return ConstVectorIterator(m_p - n);
}

template<typename Vec>
constexpr typename ConstVectorIterator<Vec>::difference_type ConstVectorIterator<Vec>::operator-(ConstVectorIterator it) const noexcept
{
    return m_p - it.m_p;
}

template<typename Vec>
constexpr typename ConstVectorIterator<Vec>::const_pointer ConstVectorIterator<Vec>::getPointer() const noexcept
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

    constexpr [[nodiscard]] reference operator*() const;
    constexpr [[nodiscard]] pointer operator->() const;
    constexpr [[nodiscard]] reference operator[](difference_type n) const;
    constexpr VectorIterator operator++() noexcept;
    constexpr VectorIterator operator++(int) noexcept;
    constexpr VectorIterator operator--() noexcept;
    constexpr VectorIterator operator--(int) noexcept;
    constexpr VectorIterator operator+=(difference_type n) noexcept;
    constexpr VectorIterator operator-=(difference_type n) noexcept;

    constexpr difference_type operator-(VectorIterator n) const noexcept;
    constexpr [[nodiscard]] VectorIterator operator-(difference_type n) const noexcept;

    constexpr [[nodiscard]] pointer getPointer() noexcept;

    friend constexpr [[nodiscard]] VectorIterator operator+(VectorIterator it, difference_type n) noexcept
    {
        return VectorIterator(it.m_p + n);
    }
};

template<typename Vec>
constexpr typename VectorIterator<Vec>::reference VectorIterator<Vec>::operator*() const
{
    return const_cast<reference>(Base::operator*());
}

template<typename Vec>
constexpr typename VectorIterator<Vec>::pointer VectorIterator<Vec>::operator->() const
{
    return const_cast<pointer>(Base::operator->());
}

template<typename Vec>
constexpr typename VectorIterator<Vec>::reference VectorIterator<Vec>::operator[](difference_type n) const
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
constexpr VectorIterator<Vec> VectorIterator<Vec>::operator-(difference_type n) const noexcept
{
    return VectorIterator(this->m_p - n);
}

template<typename Vec>
constexpr typename VectorIterator<Vec>::difference_type VectorIterator<Vec>::operator-(VectorIterator it) const noexcept
{
    return this->m_p - it.m_p;
}

template<typename Vec>
constexpr typename VectorIterator<Vec>::pointer VectorIterator<Vec>::getPointer() noexcept
{
    return this->m_p;
}

/// @brief Container of elements stored contiguously
/// @tparam T Type of elements
/// @tparam AllocationManager Allocator Helper using for storage management
///     and objects creation/deletion
template<typename T, IAllocationManagerImpl AllocationManager = CtorStratAllocationManagerT<T>>
class Vector
{
public:
    static_assert(std::is_same_v<T, typename AllocationManager::value_type>, "Types T and AllocationManager::value_type are not the same");

    using TestType = char;

    using allocator_type = typename AllocationManager::allocator_type;
    using value_type = typename allocator_type::value_type;
    using pointer = typename allocator_type::pointer;
    using const_pointer = const T*;
    using size_type = typename allocator_type::size_type;
    using difference_type = typename allocator_type::difference_type;
    using constructor_allocator = typename allocator_type::constructor_allocator;

    using reference = value_type&;
    using const_reference = const value_type&;

    using iterator = VectorIterator<Vector<value_type, AllocationManager>>;
    using const_iterator = ConstVectorIterator<Vector<value_type, AllocationManager>>;

    using prefer_init_against_ctor = std::true_type;

    constexpr Vector() = default;
    AGS_CS_ALWAYS_INLINE constexpr Vector(const Vector& rhs);
    AGS_CS_ALWAYS_INLINE constexpr Vector(Vector&& rhs) noexcept;
    AGS_CS_ALWAYS_INLINE constexpr Vector& operator=(const Vector& rhs);
    AGS_CS_ALWAYS_INLINE constexpr Vector& operator=(Vector&& rhs) noexcept;
    AGS_CS_ALWAYS_INLINE constexpr ~Vector() noexcept;

    /// @brief Copy init method
    /// @param rhs Another instance
    /// @return Status of initialization
    constexpr Status init(const Vector& rhs);

    /// @brief Move init method
    /// @param rhs Another instance
    /// @return Status of initialization
    constexpr Status init(Vector&& rhs) noexcept;

    /// @brief Set size of data
    /// @note For trivially copyable types.
    ///     Only set data size, no default values are set.
    /// @param n New size
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status setSize(size_type n) noexcept
        requires std::is_trivially_copyable_v<T>;

    /// @brief Preallocate at least that much memory that
    ///     is enough to hold N elements of T
    /// @param n Number of elements that underlying storage
    ///     must be capable to hold
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status reserve(size_type n);
    
    /// @brief Append element to tail of the storage
    /// @param value Value that need to append
    /// @return Status of operation
    constexpr Status pushBack(const T& value);

    /// @brief Append element to tail of the storage
    /// @param value Value that need to move to storage
    /// @return Status of operation
    constexpr Status pushBack(T&& value);

    /// @brief Append N elements to tail of the storage
    /// @param p Pointer to array of elements
    /// @param n Number of elements that need to append
    /// @return Status of operation
    constexpr Status pushBackN(const T* p, size_type n);

    /// @brief Append arithmetic or enum value to tail of container
    /// @remark Using for eliminating redundant overhead on raw arrays
    /// @tparam V Type of value that need to append
    /// @param value Value that need to append
    /// @return Status of operation
    template<typename V>
    constexpr Status pushBackArithmeticValue(V value)
        requires std::is_same_v<T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>);

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
    constexpr Status replace(const T* p, size_type n, size_type offset, size_type* pNewOffset = nullptr);

    /// @brief Insert N elements from offset
    /// @note  
    /// @param p 
    /// @param n 
    /// @param offset 
    /// @param pNewOffset 
    /// @return 
    constexpr Status insert(const T* p, size_type n, size_type offset, size_type* pNewOffset = nullptr);
    constexpr Status insert(const T& value, size_type offset);
    template<typename ItSrc>
    constexpr Status insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin, iterator* pDestEnd = nullptr);

    constexpr Status erase(size_type offset);
    constexpr Status erase(size_type offset, size_type n);
    constexpr Status erase(iterator pos);
    constexpr Status erase(iterator destBegin, iterator destEnd);

    // copy from Vector to p (destination must be initialized (for non pod-types))
    constexpr Status copyN(size_type offset, size_type n, T* p, T** ppNew = nullptr);

    // copy from Vector to destBegin... (destination must be initialized (for non pod-types))
    template<typename ItDest>
    constexpr Status copyN(iterator srcBegin, iterator srcEnd, ItDest destBegin, ItDest* pDestEnd = nullptr);

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] T* data() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const T* data() const noexcept;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] T& operator[](size_type offset);
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const T& operator[](size_type offset) const;
    
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] T& back();
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const T& back() const;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] T& front();
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const T& front() const;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type size() const noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type max_size() const noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] size_type capacity() const noexcept;

    constexpr void clear() noexcept;
    constexpr void invalidate() noexcept;

    // you shall free memory returned by this method manually
    constexpr [[nodiscard]] T* release() noexcept;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] iterator begin() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const_iterator begin() const noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] iterator end() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const_iterator end() const noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const_iterator cbegin() const noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const_iterator cend() const noexcept;

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] AllocationManager& getAllocationManager() noexcept;
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] const AllocationManager& getAllocationManager() const noexcept;

    constexpr [[nodiscard]] bool operator==(const Vector& rhs) const noexcept
        requires (NotPointer<T>&& HasEqualityOperator<T> || NotPointer<std::remove_pointer_t<T>> && HasEqualityOperator<std::remove_pointer_t<T>>);

private:
    constexpr [[nodiscard]] Status reserveInternal(size_type n, bool strict);
    constexpr [[nodiscard]] Status addSpaceIfNeed(size_type n);
    constexpr [[nodiscard]] bool isIteratorNotDereferenceable(iterator it) const noexcept;

    T* m_p{ nullptr };
    size_type m_dataSize{ 0 };
    size_type m_allocatedSize{ 0 };

    AllocationManager m_AllocationManager;

private:
    // It's not mandatory to have friend csp::processing::TemplateProcessor class 
    // to have deserialization capability.
    // The only reason this is done is because it allows some optimizations in
    // processing when we have direct access to private fields.
    friend csp::processing::data::TemplateProcessor<Vector<T, AllocationManager>, T, AllocationManager>;
};

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Vector<T, AllocationManager>::Vector(const Vector& rhs)
{
    init(rhs);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Vector<T, AllocationManager>::Vector(Vector&& rhs) noexcept
{
    init(std::move(rhs));
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Vector<T, AllocationManager>& Vector<T, AllocationManager>::operator=(const Vector& rhs)
{
    init(rhs);

    return *this;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Vector<T, AllocationManager>& Vector<T, AllocationManager>::operator=(Vector&& rhs) noexcept
{
    init(std::move(rhs));

    return *this;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Vector<T, AllocationManager>::~Vector() noexcept
{
    invalidate();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::init(const Vector& rhs)
{
    if (this != &rhs)
    {
        clear();

        m_AllocationManager = rhs.m_AllocationManager;

        if (rhs.m_dataSize > m_allocatedSize)
        {
            m_AllocationManager.deallocate(m_p);
            m_dataSize = 0;
            m_allocatedSize = 0;

            m_p = m_AllocationManager.allocate(rhs.m_dataSize, &m_allocatedSize);

            if (!m_p)
                return Status::ErrorNoMemory;
        }

        AGS_CS_RUN(m_AllocationManager.copyToRawNoOverlap(m_p, rhs.m_p, rhs.m_dataSize));
        m_dataSize = rhs.m_dataSize;
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::init(Vector&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_AllocationManager.destroyAndDeallocate(m_p, m_dataSize);

        m_dataSize = rhs.m_dataSize;
        m_allocatedSize = rhs.m_allocatedSize;
        m_AllocationManager = std::move(rhs.m_AllocationManager);

        m_p = rhs.release();
    }

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::setSize(size_type n) noexcept
    requires std::is_trivially_copyable_v<T>
{
    AGS_CS_RUN(reserveInternal(n, false));
    m_dataSize = n;
    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::reserve(size_type n)
{
    return reserveInternal(n, true);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::pushBack(const T& value)
{
    AGS_CS_RUN(addSpaceIfNeed(1));
    AGS_CS_RUN(m_AllocationManager.construct(m_p + m_dataSize, value));
    ++m_dataSize;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::pushBack(T&& value)
{
    AGS_CS_RUN(addSpaceIfNeed(1));
    AGS_CS_RUN(m_AllocationManager.moveToRawNoOverlap(m_p + m_dataSize, &value, 1));
    ++m_dataSize;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::pushBackN(const T* p, size_type n)
{
    if (p == nullptr && n != 0)
        return Status::ErrorInvalidArgument;

    AGS_CS_RUN(addSpaceIfNeed(n));

    AGS_CS_RUN(m_AllocationManager.copyToRawNoOverlap(m_p + m_dataSize, p, n));
    m_dataSize += n;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
template<typename V>
constexpr Status Vector<T, AllocationManager>::pushBackArithmeticValue(V value)
    requires std::is_same_v<T, uint8_t> && (std::is_arithmetic_v<V> || std::is_enum_v<V>)
{
    Status status = Status::NoError;

    AGS_CS_RUN(addSpaceIfNeed(sizeof(V)));

    *static_cast<V*>(static_cast<void*>(m_p + m_dataSize)) = value;
    m_dataSize += sizeof(V);

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
template<typename... Ts>
constexpr Status Vector<T, AllocationManager>::emplaceBack(Ts&&... ts)
{
    Status status = Status::NoError;

    AGS_CS_RUN(addSpaceIfNeed(1));
    AGS_CS_RUN(m_AllocationManager.construct(m_p + m_dataSize, std::forward<Ts>(ts)...));
    ++m_dataSize;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::replace(const T* p, size_type n, size_type offset, size_type* pNewOffset)
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
        pointer newMp = m_AllocationManager.allocate(newDataSize, &newAllocatedSize);
        if (newMp)
        {
            m_allocatedSize = newAllocatedSize;
            AGS_CS_RUN(m_AllocationManager.moveToRawNoOverlap(newMp, m_p, offset));
            AGS_CS_RUN(m_AllocationManager.copyToRawNoOverlap(newMp + offset, p, n));
            m_AllocationManager.destroyAndDeallocate(m_p, m_dataSize);

            m_p = newMp;
        }
        else
            return Status::ErrorNoMemory;
    }
    else if (newDataSize >= m_dataSize) // there is no overflow
    {
        if constexpr (constructor_allocator::value)
            m_AllocationManager.destroyN(m_p + offset, n >= m_dataSize - offset ? m_dataSize - offset : n);

        AGS_CS_RUN(m_AllocationManager.copyToRawNoOverlap(m_p + offset, p, n));
    }

    m_dataSize = newDataSize;

    if (pNewOffset)
        *pNewOffset = offsetPlusN;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::insert(const T* p, size_type n, size_type offset, size_type* pNewOffset)
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
        pointer newMp = m_AllocationManager.allocate(newDataSize, &newAllocatedSize);
        if (newMp)
        {
            m_allocatedSize = newAllocatedSize;
            AGS_CS_RUN(m_AllocationManager.moveToRawNoOverlap(newMp, m_p, offset));
            AGS_CS_RUN(m_AllocationManager.copyToRawNoOverlap(newMp + offset, p, n));

            if (offset != m_dataSize) // if we do not push back
                AGS_CS_RUN(m_AllocationManager.moveToRawNoOverlap(newMp + offsetPlusN, m_p + offset, rightDataPartSize));

            m_AllocationManager.destroyAndDeallocate(m_p, m_dataSize);

            m_p = newMp;
        }
        else
            return Status::ErrorNoMemory;
    }
    else if (newDataSize > m_dataSize)
    {
        if (offset != m_dataSize)
            AGS_CS_RUN(m_AllocationManager.moveToDirty(m_p + offsetPlusN, m_p + m_dataSize, m_p + offset, rightDataPartSize));

        AGS_CS_RUN(m_AllocationManager.copyToRawNoOverlap(m_p + offset, p, n));
    }
    else if (newDataSize < m_dataSize)
        return Status::ErrorOverflow;

    m_dataSize = newDataSize;

    if (pNewOffset)
        *pNewOffset = offsetPlusN;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::insert(const T& value, size_type offset)
{
    return insert(&value, 1, offset);
}

template<typename T, IAllocationManagerImpl AllocationManager>
template<typename ItSrc>
constexpr Status Vector<T, AllocationManager>::insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin, iterator* pDestEnd)
{
    if (isIteratorNotDereferenceable(destBegin) && destBegin != end())
        return Status::ErrorOverflow;

    Vector<T, AllocationManager> temp;
    size_type oldDataSize = m_dataSize;
    size_type currentOffset = destBegin.getPointer() - m_p;

    while (srcBegin != srcEnd)
    {
        if (currentOffset < m_allocatedSize)
        {
            pointer pCurrent = m_p + currentOffset;
            if (m_dataSize > currentOffset)
            {
                AGS_CS_RUN(temp.pushBack(std::move(*pCurrent)));
                m_AllocationManager.destroy(pCurrent); // if T is not moveable we should destroying its objects explicitly
                ++currentOffset;
            }
            else
                m_dataSize = ++currentOffset;

            AGS_CS_RUN(m_AllocationManager.copyToRawNoOverlap(pCurrent, &*srcBegin++, 1));
        }
        else
            AGS_CS_RUN(reserveInternal(currentOffset + 1, false));
    }

    if (temp.size())
    {
        if (oldDataSize > currentOffset)
        {
            if constexpr (constructor_allocator::value)
                for (size_type i = currentOffset; i < oldDataSize; ++i)
                {
                    AGS_CS_RUN(temp.pushBack(std::move(*(m_p + i))));
                    m_AllocationManager.destroy(m_p + i); // if T is not moveable we should destroying its objects explicitly
                }
            else
                AGS_CS_RUN(temp.pushBackN(m_p + currentOffset, oldDataSize - currentOffset));
        }

        m_dataSize = currentOffset;
        AGS_CS_RUN(addSpaceIfNeed(temp.size()));

        AGS_CS_RUN(m_AllocationManager.moveToRawNoOverlap(m_p + currentOffset, temp.data(), temp.size()));
        m_dataSize = currentOffset + temp.size();
    }

    if (pDestEnd)
        *pDestEnd = iterator(m_p + currentOffset);

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::erase(size_type offset)
{
    return erase(offset, 1);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::erase(size_type offset, size_type n)
{
    if (offset > m_dataSize || offset == m_dataSize && n != 0)
        return Status::ErrorOverflow;
    else if (n == 0)
        return Status::NoError;

    if (offset + n > m_dataSize || offset + n < offset)
        n = m_dataSize - offset;

    m_AllocationManager.destroyN(m_p + offset, n);

    difference_type rightNStart = offset + n;
    difference_type rightN = m_dataSize - rightNStart;

    AGS_CS_RUN(m_AllocationManager.moveToRawNoOverlap(m_p + offset, m_p + rightNStart, rightN));
    m_AllocationManager.destroyN(m_p + rightNStart, rightN); // if T is not moveable we should destroying its objects explicitly

    m_dataSize = offset + rightN;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::erase(iterator pos)
{
    return erase(pos, pos + 1);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::erase(iterator destBegin, iterator destEnd)
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

    m_AllocationManager.destroyN(destBegin.getPointer(), n);

    difference_type leftN = destBegin - begin();
    difference_type rightN = m_dataSize - n - leftN;

    AGS_CS_RUN(m_AllocationManager.moveToRawNoOverlap(destBegin.getPointer(), destEnd.getPointer(), rightN));
    m_AllocationManager.destroyN(destEnd.getPointer(), rightN); // if T is not moveable we should destroying its objects explicitly

    m_dataSize = leftN + rightN;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::copyN(size_type offset, size_type n, T* p, T** ppNew)
{
    if (p == nullptr && n != 0)
        return Status::ErrorInvalidArgument;

    if (offset > m_dataSize || offset == m_dataSize && n != 0)
        return Status::ErrorOverflow;

    difference_type nReal = offset + n > m_dataSize ? m_dataSize - offset : n;

    AGS_CS_RUN(m_AllocationManager.copyToDirty(p, p + nReal, m_p + offset, nReal));

    if (ppNew)
        *ppNew = p + nReal;

    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
template<typename ItDest>
constexpr Status Vector<T, AllocationManager>::copyN(iterator srcBegin, iterator srcEnd, ItDest destBegin, ItDest* pDestEnd)
{
    if (isIteratorNotDereferenceable(srcBegin))
        return Status::ErrorInvalidArgument;

    if (srcBegin > srcEnd)
        return Status::ErrorOverflow;

    if (srcEnd > end())
        srcEnd = end();

    while (srcBegin != srcEnd)
    {
        AGS_CS_RUN(m_AllocationManager.copyToDirty(&*destBegin, &*destBegin + 1, srcBegin++.getPointer(), 1));
        ++destBegin;
    }

    if (pDestEnd)
        *pDestEnd = destBegin;
    
    return Status::NoError;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr T* Vector<T, AllocationManager>::data() noexcept
{
    return m_p;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr const T* Vector<T, AllocationManager>::data() const noexcept
{
    return m_p;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr T& Vector<T, AllocationManager>::operator[](size_type offset)
{
    return *(m_p + offset);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr const T& Vector<T, AllocationManager>::operator[](size_type offset) const
{
    return *(m_p + offset);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr T& Vector<T, AllocationManager>::back()
{
    return *(m_p + size() - 1);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr const T& Vector<T, AllocationManager>::back() const
{
    return *(m_p + size() - 1);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr T& Vector<T, AllocationManager>::front()
{
    return *m_p;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr const T& Vector<T, AllocationManager>::front() const
{
    return *m_p;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Vector<T, AllocationManager>::size_type Vector<T, AllocationManager>::size() const noexcept
{
    return m_dataSize;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Vector<T, AllocationManager>::size_type Vector<T, AllocationManager>::max_size() const noexcept
{
    return m_AllocationManager.max_size();
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Vector<T, AllocationManager>::size_type Vector<T, AllocationManager>::capacity() const noexcept
{
    return m_allocatedSize;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr void Vector<T, AllocationManager>::clear() noexcept
{
    m_AllocationManager.destroyN(m_p, m_dataSize);
    m_dataSize = 0;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr void Vector<T, AllocationManager>::invalidate() noexcept
{
    clear();
    m_AllocationManager.deallocate(m_p);
    m_p = nullptr;
    m_allocatedSize = 0;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr T* Vector<T, AllocationManager>::release() noexcept
{
    T* s = m_p;
    m_p = 0;
    m_dataSize = 0;
    m_allocatedSize = 0;
    return s;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Vector<T, AllocationManager>::iterator Vector<T, AllocationManager>::begin() noexcept
{
    return iterator(m_p);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Vector<T, AllocationManager>::const_iterator Vector<T, AllocationManager>::begin() const noexcept
{
    return const_iterator(m_p);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Vector<T, AllocationManager>::iterator Vector<T, AllocationManager>::end() noexcept
{
    return iterator(m_p + m_dataSize);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Vector<T, AllocationManager>::const_iterator Vector<T, AllocationManager>::end() const noexcept
{
    return const_iterator(m_p + m_dataSize);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Vector<T, AllocationManager>::const_iterator Vector<T, AllocationManager>::cbegin() const noexcept
{
    return const_iterator(m_p);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr typename Vector<T, AllocationManager>::const_iterator Vector<T, AllocationManager>::cend() const noexcept
{
    return const_iterator(m_p + m_dataSize);
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr AllocationManager& Vector<T, AllocationManager>::getAllocationManager() noexcept
{
    return m_AllocationManager;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr const AllocationManager& Vector<T, AllocationManager>::getAllocationManager() const noexcept
{
    return m_AllocationManager;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::reserveInternal(size_type n, bool strict)
{
    if (n > m_allocatedSize)
    {
        T* pNewMp = strict ? m_AllocationManager.allocateStrict(n) : m_AllocationManager.allocate(n, &n);
        if (pNewMp)
        {
            m_allocatedSize = n;

            if (Status status = m_AllocationManager.moveToRawNoOverlap(pNewMp, m_p, m_dataSize); !statusSuccess(status))
            {
                m_dataSize = 0;
                m_AllocationManager.deallocate(pNewMp);
                return status;
            }

            m_AllocationManager.destroyAndDeallocate(m_p, m_dataSize);
            m_p = pNewMp;
        }
        else
            return Status::ErrorNoMemory;
    }

    return Status::NoError;
}


template<typename T, IAllocationManagerImpl AllocationManager>
constexpr Status Vector<T, AllocationManager>::addSpaceIfNeed(size_type n)
{
    return m_dataSize + n >= m_dataSize
        ? m_dataSize + n > m_allocatedSize
            ? reserveInternal(m_dataSize + n, false)
            : Status::NoError
        : Status::ErrorOverflow;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr bool Vector<T, AllocationManager>::isIteratorNotDereferenceable(iterator it) const noexcept
{
    return &*it < m_p || &*it >= m_p + m_dataSize;
}

template<typename T, IAllocationManagerImpl AllocationManager>
constexpr bool Vector<T, AllocationManager>::operator==(const Vector& rhs) const noexcept
    requires (NotPointer<T> && HasEqualityOperator<T> || NotPointer<std::remove_pointer_t<T>> && HasEqualityOperator<std::remove_pointer_t<T>>)
{
    if (size() != rhs.size())
        return false;

    for (size_type i = 0; i < size(); ++i)
        if constexpr (NotPointer<T>)
        {
            if (m_p[i] != rhs.m_p[i])
                return false;
        }
        else // if constexpr (NotPointer<std::remove_pointer_t<T>>)
        {
            if (m_p[i] == nullptr || rhs.m_p[i] == nullptr)
                return m_p[i] == rhs.m_p[i];
            
            if (*m_p[i] != *rhs.m_p[i])
                return false;
        }

    return true;
}

} // namespace common_serialization
