/**
 * @file Vector.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin
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

#include "Allocators/ConstructorNoexceptAllocator.h"
#include "Allocators/AllocatorHelpers/StrategicAllocatorHelper.h"
#include "Allocators/AllocatorHelpers/AllocatorHelperConcepts.h"

namespace common_serialization
{

template<typename Vec>
class ConstVectorIterator
{
public:
    using value_type = typename Vec::value_type;
    using pointer = typename Vec::pointer;
    using const_pointer = typename Vec::const_pointer;
    using const_reference = typename Vec::const_reference;
    using difference_type = typename Vec::difference_type;

    ConstVectorIterator(pointer p) : m_p(p) { }
    ConstVectorIterator(const ConstVectorIterator& it) : m_p(it.m_p) { }

    [[nodiscard]] constexpr inline bool operator==(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr inline bool operator!=(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr inline bool operator<(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr inline bool operator>=(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr inline bool operator>(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr inline bool operator<=(const ConstVectorIterator& rhs) const noexcept;
    [[nodiscard]] constexpr inline const_reference operator*() const noexcept;
    [[nodiscard]] constexpr inline const_pointer operator->() const noexcept;
    [[nodiscard]] constexpr inline const_reference operator[](difference_type n) const noexcept;
    constexpr inline ConstVectorIterator operator++() noexcept;
    constexpr inline ConstVectorIterator operator++(int) noexcept;
    constexpr inline ConstVectorIterator operator--() noexcept;
    constexpr inline ConstVectorIterator operator--(int) noexcept;
    constexpr inline ConstVectorIterator operator+=(difference_type n) noexcept;
    constexpr inline ConstVectorIterator operator-=(difference_type n) noexcept;
    
    [[nodiscard]] constexpr inline ConstVectorIterator operator-(difference_type n) const noexcept;
    [[nodiscard]] constexpr inline difference_type operator-(ConstVectorIterator it) const noexcept;

    [[nodiscard]] constexpr inline const_pointer getPointer() const noexcept;

    [[nodiscard]] friend constexpr inline ConstVectorIterator operator+(ConstVectorIterator it, difference_type n) noexcept
    {
        return it.m_p + n;
    }

protected:
    pointer m_p = nullptr;
};

template<typename Vec>
[[nodiscard]] constexpr inline bool ConstVectorIterator<Vec>::operator==(const ConstVectorIterator& rhs) const noexcept
{
    return m_p == rhs.m_p;
}

template<typename Vec>
[[nodiscard]] constexpr inline bool ConstVectorIterator<Vec>::operator!=(const ConstVectorIterator& rhs) const noexcept
{
    return !operator==(rhs);
}

template<typename Vec>
[[nodiscard]] constexpr inline bool ConstVectorIterator<Vec>::operator<(const ConstVectorIterator& rhs) const noexcept
{
    return rhs.m_p > m_p;
}

template<typename Vec>
[[nodiscard]] constexpr inline bool ConstVectorIterator<Vec>::operator>=(const ConstVectorIterator& rhs) const noexcept
{
    return !operator<(rhs);
}

template<typename Vec>
[[nodiscard]] constexpr inline bool ConstVectorIterator<Vec>::operator>(const ConstVectorIterator& rhs) const noexcept
{
    return rhs.m_p < m_p;
}

template<typename Vec>
[[nodiscard]] constexpr inline bool ConstVectorIterator<Vec>::operator<=(const ConstVectorIterator& rhs) const noexcept
{
    return !operator>(rhs);
}

template<typename Vec>
[[nodiscard]] constexpr inline ConstVectorIterator<Vec>::const_reference ConstVectorIterator<Vec>::operator*() const noexcept
{
    return *m_p;
}

template<typename Vec>
[[nodiscard]] constexpr inline ConstVectorIterator<Vec>::const_pointer ConstVectorIterator<Vec>::operator->() const noexcept
{
    return m_p;
}

template<typename Vec>
[[nodiscard]] constexpr inline ConstVectorIterator<Vec>::const_reference ConstVectorIterator<Vec>::operator[](difference_type n) const noexcept
{
    return *(m_p + n);
}

template<typename Vec>
constexpr inline ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator++() noexcept
{
    return ++m_p, * this;
}

template<typename Vec>
constexpr inline ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator++(int) noexcept
{
    return ConstVectorIterator(m_p++);
}

template<typename Vec>
constexpr inline ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator--() noexcept
{
    return --m_p, * this;
}

template<typename Vec>
constexpr inline ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator--(int) noexcept
{
    return ConstVectorIterator(m_p--);
}

template<typename Vec>
constexpr inline ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator+=(difference_type n) noexcept
{
    return m_p += n, *this;
}

template<typename Vec>
constexpr inline ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator-=(difference_type n) noexcept
{
    return m_p -= n, *this;
}

template<typename Vec>
[[nodiscard]] constexpr inline ConstVectorIterator<Vec> ConstVectorIterator<Vec>::operator-(difference_type n) const noexcept
{
    return ConstVectorIterator(m_p - n);
}

template<typename Vec>
[[nodiscard]] constexpr inline ConstVectorIterator<Vec>::difference_type ConstVectorIterator<Vec>::operator-(ConstVectorIterator it) const noexcept
{
    return m_p - it.m_p;
}

template<typename Vec>
[[nodiscard]] constexpr inline ConstVectorIterator<Vec>::const_pointer ConstVectorIterator<Vec>::getPointer() const noexcept
{
    return m_p;
}

template<typename Vec>
class VectorIterator : public ConstVectorIterator<Vec>
{
    using Base = ConstVectorIterator<Vec>;
public:
    using value_type = typename Vec::value_type;
    using pointer = typename Vec::pointer;   
    using reference = typename Vec::reference;
    using difference_type = typename Vec::difference_type;

    VectorIterator(pointer p) : Base(p) { }
    VectorIterator(const VectorIterator& it) : Base(it.m_p) { }
    VectorIterator(const Base& it) : Base(it.m_p) { }

    [[nodiscard]] constexpr inline reference operator*() const noexcept;
    [[nodiscard]] constexpr inline pointer operator->() const noexcept;
    [[nodiscard]] constexpr inline reference operator[](difference_type n) const noexcept;
    constexpr inline VectorIterator operator++() noexcept;
    constexpr inline VectorIterator operator++(int) noexcept;
    constexpr inline VectorIterator operator--() noexcept;
    constexpr inline VectorIterator operator--(int) noexcept;
    constexpr inline VectorIterator operator+=(difference_type n) noexcept;
    constexpr inline VectorIterator operator-=(difference_type n) noexcept;

    constexpr inline difference_type operator-(VectorIterator n) const noexcept;
    [[nodiscard]] constexpr inline VectorIterator operator-(difference_type n) const noexcept;

    [[nodiscard]] constexpr inline pointer getPointer() noexcept;

    [[nodiscard]] friend constexpr inline VectorIterator operator+(VectorIterator it, difference_type n) noexcept
    {
        return it.m_p + n;
    }
};

template<typename Vec>
[[nodiscard]] constexpr inline VectorIterator<Vec>::reference VectorIterator<Vec>::operator*() const noexcept
{
    return const_cast<reference>(Base::operator*());
}

template<typename Vec>
[[nodiscard]] constexpr inline VectorIterator<Vec>::pointer VectorIterator<Vec>::operator->() const noexcept
{
    return const_cast<pointer>(Base::operator->());
}

template<typename Vec>
[[nodiscard]] constexpr inline VectorIterator<Vec>::reference VectorIterator<Vec>::operator[](difference_type n) const noexcept
{
    return const_cast<reference>(Base::operator[](n));
}

template<typename Vec>
constexpr inline VectorIterator<Vec> VectorIterator<Vec>::operator++() noexcept
{
    return ++this->m_p, * this;
}

template<typename Vec>
constexpr inline VectorIterator<Vec> VectorIterator<Vec>::operator++(int) noexcept
{
    return VectorIterator(this->m_p++);
}

template<typename Vec>
constexpr inline VectorIterator<Vec> VectorIterator<Vec>::operator--() noexcept
{
    return --this->m_p, *this;
}

template<typename Vec>
constexpr inline VectorIterator<Vec> VectorIterator<Vec>::operator--(int) noexcept
{
    return VectorIterator(this->m_p--);
}

template<typename Vec>
constexpr inline VectorIterator<Vec> VectorIterator<Vec>::operator+=(difference_type n) noexcept
{
    return this->m_p += n, *this;
}

template<typename Vec>
constexpr inline VectorIterator<Vec> VectorIterator<Vec>::operator-=(difference_type n) noexcept
{
    return this->m_p -= n, *this;
}

template<typename Vec>
[[nodiscard]] constexpr inline VectorIterator<Vec> VectorIterator<Vec>::operator-(difference_type n) const noexcept
{
    return VectorIterator(this->m_p - n);
}

template<typename Vec>
[[nodiscard]] constexpr inline VectorIterator<Vec>::difference_type VectorIterator<Vec>::operator-(VectorIterator it) const noexcept
{
    return this->m_p - it.m_p;
}

template<typename Vec>
[[nodiscard]] constexpr inline VectorIterator<Vec>::pointer VectorIterator<Vec>::getPointer() noexcept
{
    return this->m_p;
}


/// <summary>
/// Vector - is a container class that has contiguous array as data keeper
/// </summary>
/// <typeparam name="T">type of data stored in contiguous array</typeparam>
/// <typeparam name="AllocatorHelper">class that implements InterfaceAllocatorHelper</typeparam>
template<typename T, typename AllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T, RawHeapAllocator>>>
class Vector
{
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = VectorIterator<Vector<value_type, AllocatorHelper>>;
    using const_iterator = ConstVectorIterator<Vector<value_type, AllocatorHelper>>;

    constexpr inline Vector() noexcept;
    constexpr inline Vector(const Vector& rhs);
    constexpr inline Vector(Vector&& rhs) noexcept;
    constexpr Vector& operator=(const Vector& rhs);
    constexpr Vector& operator=(Vector&& rhs) noexcept;
    constexpr inline ~Vector() noexcept;

    constexpr inline bool reserve(size_type n);
    
    constexpr Vector& push_back(const T& value);
    constexpr Vector& push_back(T&& value);
    constexpr Vector& push_back_n(const T* p, size_type n);

    constexpr size_type replace(size_type offset, const T* p, size_type n);

    constexpr size_type insert(size_type offset, const T* p, size_type n); 
    template<typename It_src>
    constexpr iterator insert(iterator destBegin, It_src srcBegin, It_src srcEnd);

    constexpr size_type erase(size_type offset, size_type n);
    constexpr iterator erase(iterator destBegin, iterator destEnd);

    //constexpr size_type copy_n(const T* p, size_type n, size_type offset);

    [[nodiscard]] constexpr inline T* data() noexcept;
    [[nodiscard]] constexpr inline const T* data() const noexcept;
    [[nodiscard]] constexpr inline T& operator[](size_type offset);
    [[nodiscard]] constexpr inline const T& operator[](size_type offset) const;

    [[nodiscard]] constexpr inline size_type size() const noexcept;
    [[nodiscard]] constexpr inline size_type max_size() const noexcept;
    [[nodiscard]] constexpr inline size_type capacity() const noexcept;

    constexpr void clear() noexcept;
    constexpr void invalidate() noexcept;

    [[nodiscard]] constexpr T* release() noexcept;

    [[nodiscard]] constexpr iterator begin() noexcept;
    [[nodiscard]] constexpr iterator end() noexcept;
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept;
    [[nodiscard]] constexpr const_iterator cend() const noexcept;

    constexpr inline AllocatorHelper& getAllocatorHelper() noexcept;
    constexpr inline const AllocatorHelper& getAllocatorHelper() const noexcept;

private:
    constexpr bool reserveInternal(size_type n, bool strict);
    constexpr inline bool addSpaceIfNeed(size_type n);
    constexpr inline bool notEndIterator(iterator it) const noexcept;

    T* m_p = nullptr;
    size_type m_dataSize = 0;
    size_type m_allocatedSize = 0;

    AllocatorHelper m_allocatorHelper;
};

template<typename T, typename AllocatorHelper>
constexpr inline Vector<T, AllocatorHelper>::Vector() noexcept
{ }

template<typename T, typename AllocatorHelper>
constexpr inline Vector<T, AllocatorHelper>::Vector(const Vector<T, AllocatorHelper>& rhs)
{
    operator=(rhs);
}

template<typename T, typename AllocatorHelper>
constexpr inline Vector<T, AllocatorHelper>::Vector(Vector<T, AllocatorHelper>&& rhs) noexcept
{
    operator=(std::move(rhs));
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>& Vector<T, AllocatorHelper>::operator=(const Vector<T, AllocatorHelper>& rhs)
{
    if (this != &rhs)
    {
        m_allocatorHelper = rhs.m_allocatorHelper;

        m_allocatorHelper.destroyN(m_p, m_dataSize);

        if (rhs.m_dataSize > m_allocatedSize)
        {
            m_allocatorHelper.deallocate(m_p);
            m_dataSize = 0;
            m_allocatedSize = 0;

            m_p = m_allocatorHelper.allocate(rhs.m_dataSize, &m_allocatedSize);
        }

        if (m_p)
        {
            m_allocatorHelper.copyNoOverlap(m_p, rhs.m_p, rhs.m_dataSize);
            m_dataSize = rhs.m_dataSize;
        }
    }

    return *this;
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>& Vector<T, AllocatorHelper>::operator=(Vector<T, AllocatorHelper>&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_allocatorHelper.destroyAndDeallocate(m_p, m_dataSize);

        m_dataSize = rhs.m_dataSize;
        m_allocatedSize = rhs.m_allocatedSize;
        m_allocatorHelper = rhs.m_allocatorHelper;

        m_p = rhs.release();
    }

    return *this;
}

template<typename T, typename AllocatorHelper>
constexpr inline Vector<T, AllocatorHelper>::~Vector() noexcept
{
    m_allocatorHelper.destroyAndDeallocate(m_p, m_dataSize);
}

template<typename T, typename AllocatorHelper>
constexpr void Vector<T, AllocatorHelper>::clear() noexcept
{
    m_allocatorHelper.destroyN(m_p, m_dataSize);
    m_dataSize = 0;
}

template<typename T, typename AllocatorHelper>
constexpr void Vector<T, AllocatorHelper>::invalidate() noexcept
{
    clear();
    m_p = nullptr;
    m_allocatedSize = 0;
}

template<typename T, typename AllocatorHelper>
constexpr inline bool Vector<T, AllocatorHelper>::reserve(size_type n)
{
    return reserveInternal(n, true);
}

template<typename T, typename AllocatorHelper>
constexpr bool Vector<T, AllocatorHelper>::reserveInternal(size_type n, bool strict)
{
    bool success = true;

    if (n > m_allocatedSize)
    {
        T* newMp = strict ? m_allocatorHelper.allocateStrict(n) : m_allocatorHelper.allocate(n, &m_allocatedSize);
        if (newMp)
        {
            if (strict)
                m_allocatedSize = n;

            m_allocatorHelper.moveNoOverlap(newMp, m_p, m_dataSize);
            m_allocatorHelper.destroyAndDeallocate(m_p, m_dataSize);
            m_p = newMp;
        }
        else
            success = false;
    }

    return success;
}


template<typename T, typename AllocatorHelper>
constexpr inline bool Vector<T, AllocatorHelper>::addSpaceIfNeed(size_type n)
{
    return m_dataSize + n >= m_dataSize
        ? m_dataSize + n > m_allocatedSize 
            ? reserveInternal(m_dataSize + n, false) 
            : true
        : false; // overflow
}

template<typename T, typename AllocatorHelper>
constexpr inline bool Vector<T, AllocatorHelper>::notEndIterator(iterator it) const noexcept
{
    return &*it >= m_p && &*it < m_p + m_dataSize;
}

template<typename T, typename AllocatorHelper>
constexpr inline AllocatorHelper& Vector<T, AllocatorHelper>::getAllocatorHelper() noexcept
{
    return m_allocatorHelper;
}

template<typename T, typename AllocatorHelper>
constexpr inline const AllocatorHelper& Vector<T, AllocatorHelper>::getAllocatorHelper() const noexcept
{
    return m_allocatorHelper;
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>& Vector<T, AllocatorHelper>::push_back(const T& value)
{
    if (addSpaceIfNeed(1))
    {
        m_allocatorHelper.copyNoOverlap(m_p + m_dataSize, &value, 1);
        ++m_dataSize;
    }

    return *this;
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>& Vector<T, AllocatorHelper>::push_back(T&& value)
{
    if (addSpaceIfNeed(1))
    {
        m_allocatorHelper.moveNoOverlap(m_p + m_dataSize, &value, 1);
        ++m_dataSize;
    }

    return *this;
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>& Vector<T, AllocatorHelper>::push_back_n(const T* p, size_type n)
{
    if (p && addSpaceIfNeed(n))
    {
        m_allocatorHelper.copyNoOverlap(m_p + m_dataSize, p, n);
        m_dataSize += n;
    }

    return *this;
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>::size_type Vector<T, AllocatorHelper>::replace(size_type offset, const T* p, size_type n)
{
    if (!p || !n)
        return offset;

    // don't allow to create sparse array by this function
    if (offset > m_dataSize)
        return m_dataSize;

    size_type offsetPlusN = offset + n;
    size_type newDataSize = offsetPlusN > m_dataSize ? offsetPlusN : m_dataSize;
    bool success = false;

    // if newDataSize <= m_allocatedSize there is no need to allocate memory or there is overflow
    if (newDataSize > m_allocatedSize)
    {
        size_type newAllocatedSize = 0;
        pointer newMp = m_allocatorHelper.allocate(newDataSize, &newAllocatedSize);
        if (newMp)
        {
            m_allocatedSize = newAllocatedSize;
            m_allocatorHelper.moveNoOverlap(newMp, m_p, offset);
            m_allocatorHelper.copyNoOverlap(newMp + offset, p, n);
            m_allocatorHelper.destroyAndDeallocate(m_p, m_dataSize);

            m_p = newMp;
            success = true;
        }
    }
    else if (newDataSize >= m_dataSize) // there is no overflow
    {
        if constexpr (ConstructorAllocator<typename AllocatorHelper::allocator>)
            m_allocatorHelper.destroyN(m_p + offset, n >= m_dataSize - offset ? m_dataSize - offset : n);

        m_allocatorHelper.copyNoOverlap(m_p + offset, p, n);
        success = true;
    }

    if (success)
        m_dataSize = newDataSize;

    return success ? offsetPlusN : offset;
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>::size_type Vector<T, AllocatorHelper>::insert(size_type offset, const T* p, size_type n)
{
    if (!p || !n)
        return offset;

    // don't allow to create sparse array by this function
    if (offset > m_dataSize)
        return m_dataSize;

    size_type offsetPlusN = offset + n;
    size_type newDataSize = m_dataSize + n;
    size_type rightDataPartSize = m_dataSize - offset;
    bool success = false;

    // if newDataSize <= m_allocatedSize there is no need to allocate memory or there is overflow
    if (newDataSize > m_allocatedSize)
    {
        size_type newAllocatedSize = 0;
        pointer newMp = m_allocatorHelper.allocate(newDataSize, &newAllocatedSize);
        if (newMp)
        {
            m_allocatedSize = newAllocatedSize;
            m_allocatorHelper.moveNoOverlap(newMp, m_p, offset);
            m_allocatorHelper.copyNoOverlap(newMp + offset, p, n);

            if (offset != m_dataSize) // if we do not push back
                m_allocatorHelper.moveNoOverlap(newMp + offsetPlusN, m_p + offset, rightDataPartSize);

            m_allocatorHelper.destroyAndDeallocate(m_p, m_dataSize);

            m_p = newMp;
            success = true;
        }
    }
    else if (newDataSize > m_dataSize) // there is no overflow
    {
        if (offset != m_dataSize)
            m_allocatorHelper.move(m_p + offsetPlusN, m_p + offset, rightDataPartSize);

        if constexpr (ConstructorAllocator<typename AllocatorHelper::allocator>)
            m_allocatorHelper.destroyN(m_p + offset, n >= rightDataPartSize ? rightDataPartSize : n);

        m_allocatorHelper.copyNoOverlap(m_p + offset, p, n);
        success = true;
    }

    if (success)
        m_dataSize = newDataSize;

    return success ? offsetPlusN : offset;
}

template<typename T, typename AllocatorHelper>
template<typename It_src>
constexpr Vector<T, AllocatorHelper>::iterator Vector<T, AllocatorHelper>::insert(iterator destBegin, It_src srcBegin, It_src srcEnd)
{
    if (!notEndIterator(destBegin))
        return end();

    Vector<T, AllocatorHelper> temp;
    size_type oldDataSize = m_dataSize;
    size_type currentOffset = destBegin.getPointer() - m_p;
    iterator it = destBegin;

    while (srcBegin != srcEnd)
    {
        if (currentOffset < m_allocatedSize || addSpaceIfNeed(1))
        {
            pointer pCurrent = m_p + currentOffset;
            if (m_dataSize > currentOffset)
            {
                temp.push_back(std::move(*pCurrent));
                m_allocatorHelper.destroy(pCurrent); // if T is not moveable we should destroying its objects explicitly
            }
            else
                m_dataSize = currentOffset;

            m_allocatorHelper.copyNoOverlap(pCurrent, &*srcBegin++, 1);
            ++currentOffset;
        }
        else
            break;
    }

    if (temp.size())
    {
        if (oldDataSize > currentOffset)
        {
            if constexpr (ConstructorAllocator<typename AllocatorHelper::allocator>)
                for (size_type i = currentOffset; i < oldDataSize; ++i)
                    temp.push_back(std::move(*(m_p + i)));
            else
                temp.push_back_n(m_p + currentOffset, oldDataSize - currentOffset);
        }

        if (addSpaceIfNeed(temp.size()))
        {
            
            m_allocatorHelper.moveNoOverlap(m_p + currentOffset, temp.data(), temp.size());
            m_dataSize = currentOffset + temp.size();
        }

        it = m_p + currentOffset;
    }

    return it;
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>::size_type Vector<T, AllocatorHelper>::erase(size_type offset, size_type n)
{
    if (!n)
        return offset;

    if (offset > m_dataSize)
        return m_dataSize;

    if (offset + n > m_dataSize)
        n = m_dataSize - offset;

    m_allocatorHelper.destroyN(m_p + offset, n);

    difference_type rightNStart = offset + n;
    difference_type rightN = m_dataSize - rightNStart;

    m_allocatorHelper.move(m_p + offset, m_p + rightNStart, rightN);

    m_dataSize = offset + rightN;

    return offset;
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>::iterator Vector<T, AllocatorHelper>::erase(iterator destBegin, iterator destEnd)
{
    if (destBegin > destEnd || !notEndIterator(destBegin))
        return end();

    if (destBegin == destEnd)
        return destEnd;

    if (destEnd > end())
        destEnd = end();

    difference_type n = destEnd - destBegin;

    m_allocatorHelper.destroyN(destBegin.getPointer(), n);

    difference_type leftN = destBegin - begin();
    difference_type rightN = m_dataSize - n - leftN;

    m_allocatorHelper.move(destBegin.getPointer(), destEnd.getPointer(), rightN);

    m_dataSize = leftN + rightN;

    return m_p + leftN;
}
/*
template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>::size_type Vector<T, AllocatorHelper>::copy_n(const T* p, size_type n, size_type offset)
{
    if (!p || !n)
        return offset;

    if (offset >= m_dataSize)
        return m_dataSize;

    difference_type nReal = offset + n > m_dataSize ? m_dataSize - offset : n;

    m_allocatorHelper.copy(p, m_p + offset, nReal);
    return 0;
}*/


template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr inline T* Vector<T, AllocatorHelper>::data() noexcept
{
    return m_p;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr inline const T* Vector<T, AllocatorHelper>::data() const noexcept
{
    return m_p;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr inline T& Vector<T, AllocatorHelper>::operator[](size_type offset)
{
    return *(m_p + offset);
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr inline const T& Vector<T, AllocatorHelper>::operator[](size_type offset) const
{
    return *(m_p + offset);
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr inline Vector<T, AllocatorHelper>::size_type Vector<T, AllocatorHelper>::size() const noexcept
{
    return m_dataSize;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr inline Vector<T, AllocatorHelper>::size_type Vector<T, AllocatorHelper>::max_size() const noexcept
{
    return m_allocatorHelper.max_size();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr inline Vector<T, AllocatorHelper>::size_type Vector<T, AllocatorHelper>::capacity() const noexcept
{
    return m_allocatedSize;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr T* Vector<T, AllocatorHelper>::release() noexcept
{
    T* s = m_p;
    m_p = 0;
    m_dataSize = 0;
    m_allocatedSize = 0;
    return s;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Vector<T, AllocatorHelper>::iterator Vector<T, AllocatorHelper>::begin() noexcept
{
    return m_p;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Vector<T, AllocatorHelper>::iterator Vector<T, AllocatorHelper>::end() noexcept
{
    return m_p + m_dataSize;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Vector<T, AllocatorHelper>::const_iterator Vector<T, AllocatorHelper>::cbegin() const noexcept
{
    return m_p;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Vector<T, AllocatorHelper>::const_iterator Vector<T, AllocatorHelper>::cend() const noexcept
{
    return m_p + m_dataSize;
}

using RawArray = Vector<uint8_t, StrategicAllocatorHelper<uint8_t, RawHeapAllocator>>;

} // namespace common_serialization
