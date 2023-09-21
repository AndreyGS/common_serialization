/**
 * @file Walker.h
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

#include "Vector.h"

namespace common_serialization
{

template<typename T, typename AllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T>>>
class Walker
{
public:
    using value_type = typename Vector<T, AllocatorHelper>::value_type;
    using size_type = typename Vector<T, AllocatorHelper>::size_type;

    using iterator = VectorIterator<Vector<T, AllocatorHelper>>;
    using const_iterator = ConstVectorIterator<Vector<T, AllocatorHelper>>;

    constexpr Walker() noexcept;
    constexpr Walker(const Walker& rhs);
    constexpr Walker(Walker&& rhs) noexcept;
    constexpr Walker& operator=(const Walker& rhs);
    constexpr Walker& operator=(Walker&& rhs) noexcept;
    constexpr ~Walker() noexcept;
    
    constexpr Status reserve(size_type n);
    constexpr Status reserve_from_current_offset(size_type n);

    constexpr Status pushBack(const T& value);
    constexpr Status pushBack(T&& value);
    constexpr Status pushBackN(const T* p, size_type n);

    template<typename V>
    constexpr Status pushBackArithmeticValue(V value) noexcept
        requires std::is_same_v<T, uint8_t>&& std::is_arithmetic_v<V>;

    constexpr Status replace(const T* p, size_type n, size_type offset);

    constexpr Status insert(const T* p, size_type n, size_type offset);
    template<typename ItSrc>
    constexpr Status insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin);

    constexpr Status erase(size_type offset, size_type n);
    constexpr Status erase(iterator destBegin, iterator destEnd);

    constexpr Status write(const T* p, size_type n);
    constexpr Status read(T* p, size_type n, size_type* pNRead = nullptr);

    template<typename V>
    constexpr Status readArithmeticValue(V& value) noexcept
        requires std::is_same_v<T, uint8_t> && std::is_arithmetic_v<V>;

    [[nodiscard]] constexpr T* data() noexcept;
    [[nodiscard]] constexpr const T* data() const noexcept;
    [[nodiscard]] constexpr T& operator[](size_type offset) noexcept;
    [[nodiscard]] constexpr const T& operator[](size_type offset) const noexcept;

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
    constexpr Status seek(size_type offset) noexcept;

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
constexpr Status Walker<T, AllocatorHelper>::reserve(size_type n)
{
    return m_vector.reserve(n);
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::reserve_from_current_offset(size_type n)
{
    if (m_offset + n < m_offset)
        return Status::kErrorOverflow;
    else
        return m_vector.reserve(m_offset + n);
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::pushBack(const T& value)
{
    RUN(m_vector.pushBack(value));
    m_offset = m_vector.size();
    return Status::kNoError;
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::pushBack(T&& value)
{
    RUN(m_vector.pushBack(std::move(value)));
    m_offset = m_vector.size();
    return Status::kNoError;
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::pushBackN(const T* p, size_type n)
{
    RUN(m_vector.pushBackN(p, n));
    m_offset = m_vector.size();
    return Status::kNoError;
}

template<typename T, typename AllocatorHelper>
template<typename V>
constexpr Status Walker<T, AllocatorHelper>::pushBackArithmeticValue(V value) noexcept
    requires std::is_same_v<T, uint8_t>&& std::is_arithmetic_v<V>
{
    RUN(m_vector.pushBackArithmeticValue(value));
    m_offset = m_vector.size();
    return Status::kNoError;
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::replace(const T* p, size_type n, size_type offset)
{
    RUN(m_vector.replace(p, n, offset));
    m_offset += n;

    return Status::kNoError;
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::insert(const T* p, size_type n, size_type offset)
{
    size_type oldSize = m_vector.size();

    RUN(m_vector.insert(p, n, offset));
    m_offset += m_vector.size() - oldSize;

    return Status::kNoError;
}

template<typename T, typename AllocatorHelper>
template<typename ItSrc>
constexpr Status Walker<T, AllocatorHelper>::insert(ItSrc srcBegin, ItSrc srcEnd, iterator destBegin)
{
    size_type oldSize = m_vector.size();
    
    RUN(m_vector.insert(destBegin, srcBegin, srcEnd));
    m_offset += m_vector.size() - oldSize;

    return Status::kNoError;
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::erase(size_type offset, size_type n)
{
    return m_vector.erase(offset, n);
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::erase(iterator destBegin, iterator destEnd)
{
    return m_vector.erase(destBegin, destEnd);
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::write(const T* p, size_type n)
{
    return replace(m_offset, p, n);
}

template<typename T, typename AllocatorHelper>
constexpr Status Walker<T, AllocatorHelper>::read(T* p, size_type n, size_type* pNRead)
{
    T* pNew = nullptr;

    RUN(m_vector.copyN(m_offset, n, p, &pNew));
    
    size_type nReal = pNew - p;
    m_offset += nReal;

    if (pNRead)
        *pNRead = nReal;

    return Status::kNoError;
}

template<typename T, typename AllocatorHelper>
template<typename V>
constexpr Status Walker<T, AllocatorHelper>::readArithmeticValue(V& value) noexcept
    requires std::is_same_v<T, uint8_t>&& std::is_arithmetic_v<V>
{
    if (sizeof(V) + m_offset <= size())
    {
        value = *static_cast<const V*>(static_cast<const void*>(data() + m_offset));
        m_offset += sizeof(V);
        return Status::kNoError;
    }
    else
        return Status::kErrorOverflow;
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
[[nodiscard]] constexpr T& Walker<T, AllocatorHelper>::operator[](size_type offset) noexcept
{ 
    m_offset = offset + 1;
    return m_vector[offset];
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr const T& Walker<T, AllocatorHelper>::operator[](size_type offset) const noexcept
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
constexpr Status Walker<T, AllocatorHelper>::seek(size_type offset) noexcept
{
    return (m_offset = offset) <= m_vector.size() ? Status::kNoError : Status::kErrorOverflow;
}

} // namespace common_serialization
