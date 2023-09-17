/**
 * @file Walker.h
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

#include "Vector.h"

namespace common_serialization
{

template<typename T, typename AllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T>>>
class Walker
{
public:
    using size_type = typename Vector<T, AllocatorHelper>::size_type;

    using iterator = VectorIterator<Vector<T, AllocatorHelper>>;
    using const_iterator = ConstVectorIterator<Vector<T, AllocatorHelper>>;

    constexpr Walker() noexcept;
    constexpr Walker(const Walker& rhs);
    constexpr Walker(Walker&& rhs) noexcept;
    constexpr Walker& operator=(const Walker& rhs);
    constexpr Walker& operator=(Walker&& rhs) noexcept;
    constexpr ~Walker() noexcept;
    
    constexpr bool reserve(size_type n);
    constexpr bool reserve_from_current_offset(size_type n);

    constexpr size_type push_back(const T& value);
    constexpr size_type push_back(T&& value);
    constexpr size_type push_back_n(const T* p, size_type n);

    constexpr size_type replace(size_type offset, const T* p, size_type n);

    constexpr size_type insert(size_type offset, const T* p, size_type n);
    template<typename ItSrc>
    constexpr size_type insert(iterator offset, ItSrc srcBegin, ItSrc srcEnd);

    constexpr size_type erase(size_type offset, size_type n);
    constexpr size_type erase(iterator destBegin, iterator destEnd);

    constexpr size_type write(const T* p, size_type n);
    constexpr size_type read(T* p, size_type n);

    [[nodiscard]] constexpr T* data() noexcept;
    [[nodiscard]] constexpr const T* data() const noexcept;
    [[nodiscard]] constexpr T& operator[](size_type offset);
    [[nodiscard]] constexpr const T& operator[](size_type offset) const;

    [[nodiscard]] constexpr size_type size() const noexcept;
    [[nodiscard]] constexpr size_type max_size() const noexcept;
    [[nodiscard]] constexpr size_type capacity() const noexcept;

    constexpr void clear() noexcept;
    constexpr void invalidate() noexcept;
    [[nodiscard]] constexpr T* release() noexcept;

    [[nodiscard]] constexpr AllocatorHelper& getAllocatorHelper() noexcept;
    [[nodiscard]] constexpr const AllocatorHelper& getAllocatorHelper() const noexcept;

    [[nodiscard]] constexpr Vector<T, AllocatorHelper>& getVector() noexcept;
    [[nodiscard]] constexpr const Vector<T, AllocatorHelper>& getVector() const noexcept;

    [[nodiscard]] constexpr size_type tell() const noexcept;
    constexpr size_type seek(size_type offset) noexcept;

    [[nodiscard]] constexpr T* getOffsettedPointer() noexcept;
    [[nodiscard]] constexpr const T* getOffsettedPointer() const noexcept;

private:
    Vector<T, AllocatorHelper> m_vector;
    size_type m_offset = 0;
};

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::Walker() noexcept { };

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::Walker(const Walker& rhs)
    : m_vector(rhs.m_vector), m_offset(rhs.m_offset)
{ }

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::Walker(Walker&& rhs) noexcept
    : m_vector(std::move(rhs.m_vector)), m_offset(std::move(rhs.m_offset))
{ }

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>& Walker<T, AllocatorHelper>::operator=(const Walker& rhs)
{
    if (this != &rhs)
    {
        m_vector = rhs.m_vector;
        m_offset = rhs.m_offset;
    }

    return *this;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>& Walker<T, AllocatorHelper>::operator=(Walker&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_vector = std::move(rhs.m_vector);
        m_offset = std::move(rhs.m_offset);
    }

    return *this;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::~Walker() noexcept { }

template<typename T, typename AllocatorHelper>
constexpr bool Walker<T, AllocatorHelper>::reserve(size_type n)
{
    return m_vector.reserve(n);
}

template<typename T, typename AllocatorHelper>
constexpr bool Walker<T, AllocatorHelper>::reserve_from_current_offset(size_type n)
{
    // if overflow
    if (m_offset + n < m_offset)
        return false;
    else
        return m_vector.reserve(m_offset + n);
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::push_back(const T& value)
{
    size_type oldSize = m_vector.size();
    m_vector.push_back(value);
    m_offset = m_vector.size();
    return m_offset - oldSize;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::push_back(T&& value)
{
    size_type oldSize = m_vector.size();
    m_vector.push_back(std::move(value));
    m_offset = m_vector.size();
    return m_offset - oldSize;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::push_back_n(const T* p, size_type n)
{
    size_type oldSize = m_vector.size();
    m_vector.push_back_n(p, n);
    m_offset = m_vector.size();
    return m_offset - oldSize;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::replace(size_type offset, const T* p, size_type n)
{
    m_offset = m_vector.replace(offset, p, n);
    return offset > m_vector.size() ? 0 : m_offset - offset;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::insert(size_type offset, const T* p, size_type n)
{
    m_offset = m_vector.insert(offset, p, n);
    return offset > m_vector.size() ? 0 : m_offset - offset;
}

template<typename T, typename AllocatorHelper>
template<typename ItSrc>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::insert(iterator destBegin, ItSrc srcBegin, ItSrc srcEnd)
{
    size_type oldSize = m_vector.size();
    auto it = m_vector.insert(destBegin, srcBegin, srcEnd);
    m_offset = it.getPointer() - m_vector.data();
    return m_vector.size() - oldSize;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::erase(size_type offset, size_type n)
{
    size_type oldSize = m_vector.size();
    m_offset = m_vector.erase(offset, n);
    return oldSize - m_vector.size();
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::erase(iterator destBegin, iterator destEnd)
{
    size_type oldSize = m_vector.size();
    auto it = m_vector.erase(destBegin, destEnd);
    m_offset = it.getPointer() - m_vector.data();
    return oldSize - m_vector.size();
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::write(const T* p, size_type n)
{
    return replace(m_offset, p, n);
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::read(T* p, size_type n)
{
    size_type nReal = m_vector.copy_n(m_offset, n, p) - p;
    m_offset += nReal;
    return nReal;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr T* Walker<T, AllocatorHelper>::data() noexcept
{
    return m_vector.data();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr const T* Walker<T, AllocatorHelper>::data() const noexcept
{
    return m_vector.data();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr T& Walker<T, AllocatorHelper>::operator[](size_type offset)
{
    m_offset = offset + 1;
    return m_vector[offset];
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr const T& Walker<T, AllocatorHelper>::operator[](size_type offset) const
{
    m_offset = offset + 1;
    return m_vector[offset];
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::size() const noexcept
{
    return m_vector.size();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::max_size() const noexcept
{
    return m_vector.max_size();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::capacity() const noexcept
{
    return m_vector.capacity();
}

template<typename T, typename AllocatorHelper>
constexpr void Walker<T, AllocatorHelper>::clear() noexcept
{
    m_vector.clear(), m_offset = 0;
}

template<typename T, typename AllocatorHelper>
constexpr void Walker<T, AllocatorHelper>::invalidate() noexcept
{
    m_vector.invalidate(), m_offset = 0;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr T* Walker<T, AllocatorHelper>::release() noexcept
{
    return m_offset = 0, m_vector.release();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr AllocatorHelper& Walker<T, AllocatorHelper>::getAllocatorHelper() noexcept
{
    return m_vector.getAllocatorHelper();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr const AllocatorHelper& Walker<T, AllocatorHelper>::getAllocatorHelper() const noexcept
{
    return m_vector.getAllocatorHelper();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Vector<T, AllocatorHelper>& Walker<T, AllocatorHelper>::getVector() noexcept
{
    return m_vector;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr const Vector<T, AllocatorHelper>& Walker<T, AllocatorHelper>::getVector() const noexcept
{
    return m_vector;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::tell() const noexcept
{
    return m_offset;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::seek(size_type offset) noexcept
{
    return m_offset = offset <= m_vector.size() ? offset : m_vector.size();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr T* Walker<T, AllocatorHelper>::getOffsettedPointer() noexcept
{

}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr const T* Walker<T, AllocatorHelper>::getOffsettedPointer() const noexcept
{

}

} // namespace common_serialization
