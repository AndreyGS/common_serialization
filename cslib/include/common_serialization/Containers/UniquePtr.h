/**
 * @file cslib/include/common_serialization/Containers/UniquePtr.h
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

#include <common_serialization/Containers/CompressedPair.h>
#include <common_serialization/Containers/DefaultDeleter.h>

namespace common_serialization
{

/// @brief Something like std::unique_ptr
/// @note Should have more unit-tests
/// @tparam _T Type that internal pointer points to
/// @tparam _D Deleter
template<typename _T, typename _D = DefaultDeleter<_T>>
class UniquePtr
{
public:
    using element_type = _T;
    using pointer = element_type*;
    using deleter_type = _D;

    UniquePtr()
        : m_pair(zero_then_variadic_args_t{})
    {
    }

    template<typename _T2>
        requires SmartPtrConvertible<element_type, deleter_type, _T2, deleter_type>
    explicit constexpr UniquePtr(_T2* p) noexcept 
        : m_pair(zero_then_variadic_args_t{}, p)
    {
    }

    template<typename _T2, typename _D2>
        requires (SmartPtrConvertible<element_type, deleter_type, _T2, _D2> && std::is_constructible_v<deleter_type, _D2>)
    explicit constexpr UniquePtr(_T2* p, const _D2& d) noexcept
        : m_pair(one_then_variadic_args_t{}, d, p)
    {
    }

    template<typename _T2, typename _D2>
        requires (SmartPtrConvertible<element_type, deleter_type, _T2, _D2> && !std::is_reference_v<_D2> && std::is_constructible_v<deleter_type, _D2>)
    explicit constexpr UniquePtr(_T2* p, _D2&& d) noexcept
        : m_pair(one_then_variadic_args_t{}, std::move(d), p)
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    template<typename _T2, typename _D2>
        requires SmartPtrConvertible<element_type, deleter_type, _T2, _D2>
    constexpr UniquePtr(UniquePtr<_T2, _D2>&& rhs) noexcept
        : m_pair(one_then_variadic_args_t{}, rhs.getDeleter(), rhs.release())
    {
    }

    template<typename _T2, typename _D2>
        requires SmartPtrConvertible<element_type, deleter_type, _T2, _D2>
    constexpr UniquePtr& operator=(UniquePtr<_T2, _D2>&& rhs) noexcept
    {
        reset(rhs.release());
        m_pair.getFirst() = std::forward<_D2>(rhs.getDeleter());
        return *this;
    }

    constexpr UniquePtr& operator=(pointer p) noexcept
    {
        reset(p);
        return *this;
    }

    constexpr ~UniquePtr() noexcept
    {
        reset();
    }

    /// @brief Get stored pointer
    /// @return Stored pointer
    [[nodiscard]] constexpr pointer get() const noexcept
    { 
        return m_pair.value;
    }

    /// @brief Release pointer ownership
    /// @return Stored pointer
    constexpr pointer release() noexcept
    {
        pointer p = m_pair.value;
        m_pair.value = nullptr;
        return p;
    }

    /// @brief Release pointer ownership
    /// @return Stored pointer
    void reset(pointer p = nullptr)
    {
        if (m_pair.value == p)
            return;

        if (m_pair.value)
            m_pair.getFirst()(m_pair.value);

        m_pair.value = p;
    }

    template<typename _D2>
        requires SmartPtrConvertible<element_type, deleter_type, element_type, _D2>
    void swap(UniquePtr<element_type, _D2>& rhs) noexcept
    {
        pointer pTemp = m_pair.value;
        m_pair.value = rhs.m_pair.value;
        rhs.m_pair.value = pTemp;
        auto dTemp = m_pair.getFirst();
        m_pair.getFirst() = rhs.m_pair.getFirst();
        rhs.m_pair.getFirst() = dTemp;
    }

    constexpr deleter_type& getDeleter() noexcept
    {
        return m_pair.getFirst();
    }

    constexpr const deleter_type& getDeleter() const noexcept
    {
        return m_pair.getFirst();
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept
    { 
        return m_pair.value;
    }

    [[nodiscard]] constexpr element_type& operator*() const noexcept
    { 
        return *m_pair.value;
    }

    constexpr explicit operator bool() const noexcept
    {
        return static_cast<bool>(m_pair.value);
    }

private:
    CompressedPair<deleter_type, pointer> m_pair;
};

template<typename _T, typename _D>
class UniquePtr<_T[], _D>
{
public:
    using element_type = _T;
    using pointer = element_type*;
    using deleter_type = _D;

    template<typename _D2 = DefaultDeleter<element_type>>
    UniquePtr()
        : m_pair(zero_then_variadic_args_t{})
    {
    }

    template<typename _T2>
        requires SmartPtrArrConvertible<element_type, deleter_type, _T2, deleter_type>
    explicit constexpr UniquePtr(_T2* p) noexcept 
        : m_pair(zero_then_variadic_args_t{}, p)
    {
    }

    template<typename _T2, typename _D2>
        requires (SmartPtrArrConvertible<element_type, deleter_type, _T2, _D2> && std::is_constructible_v<deleter_type, _D2>)
    explicit constexpr UniquePtr(_T2* p, const _D2& d) noexcept
        : m_pair(one_then_variadic_args_t{}, d, p)
    {
    }

    template<typename _T2, typename _D2>
        requires (SmartPtrArrConvertible<element_type, deleter_type, _T2, _D2> && !std::is_reference_v<_D2> && std::is_constructible_v<deleter_type, _D2>)
    explicit constexpr UniquePtr(_T2* p, _D2&& d) noexcept
        : m_pair(one_then_variadic_args_t{}, std::move(d), p)
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    template<typename _T2, typename _D2>
        requires SmartPtrArrConvertible<element_type, deleter_type, _T2, _D2>
    constexpr UniquePtr(UniquePtr<_T2[], _D2>&& rhs) noexcept
        : m_pair(one_then_variadic_args_t{}, rhs.getDeleter(), rhs.release())
    {
    }

    template<typename _T2, typename _D2>
        requires SmartPtrArrConvertible<element_type, deleter_type, _T2, _D2>
    constexpr UniquePtr& operator=(UniquePtr<_T2[], _D2>&& rhs) noexcept
    {
        reset(rhs.release());
        m_pair.getFirst() = std::forward<_D2>(rhs.getDeleter());
        return *this;
    }

    constexpr UniquePtr& operator=(pointer p) noexcept
    {
        reset(p);
        return *this;
    }

    constexpr ~UniquePtr() noexcept
    {
        reset();
    }

    /// @brief Get stored pointer
    /// @return Stored pointer
    [[nodiscard]] constexpr pointer get() const noexcept
    { 
        return m_pair.value;
    }

    /// @brief Release pointer ownership
    /// @return Stored pointer
    constexpr pointer release() noexcept
    {
        pointer p = m_pair.value;
        m_pair.value = nullptr;
        return p;
    }

    /// @brief Release pointer ownership
    /// @return Stored pointer
    void reset(pointer p = nullptr)
    {
        if (m_pair.value == p)
            return;

        if (m_pair.value)
            m_pair.getFirst()(m_pair.value);

        m_pair.value = p;
    }

    template<typename _D2>
        requires SmartPtrConvertible<element_type, deleter_type, element_type, _D2>
    void swap(UniquePtr<element_type[], _D2> & rhs) noexcept
    {
        pointer pTemp = m_pair.value;
        m_pair.value = rhs.m_pair.value;
        rhs.m_pair.value = pTemp;
        auto dTemp = m_pair.getFirst();
        m_pair.getFirst() = rhs.m_pair.getFirst();
        rhs.m_pair.getFirst() = dTemp;
    }

    constexpr deleter_type& getDeleter() noexcept
    {
        return m_pair.getFirst();
    }

    constexpr const deleter_type& getDeleter() const noexcept
    {
        return m_pair.getFirst();
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept
    { 
        return m_pair.value;
    }

    [[nodiscard]] constexpr element_type& operator*() const noexcept
    { 
        return *m_pair.value;
    }

    constexpr explicit operator bool() const noexcept
    {
        return static_cast<bool>(m_pair.value);
    }

private:
    CompressedPair<deleter_type, pointer> m_pair;
};


template<typename _T, typename... _Ts>
    requires (!std::is_array_v<_T>)
[[nodiscard]] constexpr UniquePtr<_T> makeUnique(_Ts&&... ts)
{
    return UniquePtr<_T>(new _T{ std::forward<_Ts>(ts)... });
}

template<typename _T, typename... _Ts>
    requires (std::is_unbounded_array_v<_T>)
[[nodiscard]] constexpr UniquePtr<_T> makeUnique(size_t size, _Ts&&... ts)
{
    using Type = std::remove_extent_t<_T>;
    return UniquePtr<_T>(new Type[size]{ std::forward<_Ts>(ts)... });
}

template<typename _T, typename... _Ts>
    requires (!std::is_array_v<_T>)
[[nodiscard]] constexpr UniquePtr<_T> makeUniqueNoThrow(_Ts&&... ts)
{
    return UniquePtr<_T>(new (std::nothrow) _T{ std::forward<_Ts>(ts)... });
}

template<typename _T, typename... _Ts>
    requires (std::is_unbounded_array_v<_T>)
[[nodiscard]] constexpr UniquePtr<_T> makeUniqueNoThrow(size_t size, _Ts&&... ts)
{
    using Type = std::remove_extent_t<_T>;
    return UniquePtr<_T>(new (std::nothrow) Type[size]{ std::forward<_Ts>(ts)... });
}

template<typename _T>
    requires (!std::is_array_v<_T>)
[[nodiscard]] constexpr UniquePtr<_T> makeUniqueForOverwrite()
{
    return UniquePtr<_T>(new _T);
}

template<typename _T>
    requires (std::is_unbounded_array_v<_T>)
[[nodiscard]] constexpr UniquePtr<_T> makeUniqueForOverwrite(size_t size)
{
    using Type = std::remove_extent_t<_T>;
    return UniquePtr<_T>(new Type[size]);
}

template<typename _T>
    requires (!std::is_array_v<_T>)
[[nodiscard]] constexpr UniquePtr<_T> makeUniqueNoThrowForOverwrite()
{
    return UniquePtr<_T>(new (std::nothrow) _T);
}

template<typename _T>
    requires (std::is_unbounded_array_v<_T>)
[[nodiscard]] constexpr UniquePtr<_T> makeUniqueNoThrowForOverwrite(size_t size)
{
    using Type = std::remove_extent_t<_T>;
    return UniquePtr<_T>(new (std::nothrow) Type[size]);
}

} // namespace common_serialization
