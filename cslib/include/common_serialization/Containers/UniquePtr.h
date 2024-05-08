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

#include "common_serialization/Helpers.h"
#include "common_serialization/Allocators/AllocatorHelpers/GenericAllocatorHelper.h"
#include "common_serialization/Containers/CompressedPair.h"
#include "common_serialization/Containers/DefaultDeleter.h"

namespace common_serialization
{

/// @brief Something like std::unique_ptr
/// @note Should have more unit-tests
/// @tparam T Type that internal pointer points to
/// @tparam Dbin Deleter
template<typename T, typename Dbin = DefaultDeleter<T>>
class UniquePtr
{
public:
    UniquePtr()
        : m_pair(zero_then_variadic_args_t{})
    {
    }

    template<typename T2>
        requires SmartPtrConvertible<T, Dbin, T2, Dbin>
    explicit constexpr UniquePtr(T2* p) noexcept 
        : m_pair(zero_then_variadic_args_t{}, p)
    {
    }

    template<typename T2, typename D2>
        requires (SmartPtrConvertible<T, Dbin, T2, D2> && std::is_constructible_v<Dbin, D2>)
    explicit constexpr UniquePtr(T2* p, const D2& d) noexcept
        : m_pair(one_then_variadic_args_t{}, d, p)
    {
    }

    template<typename T2, typename D2>
        requires (SmartPtrConvertible<T, Dbin, T2, D2> && !std::is_reference_v<D2> && std::is_constructible_v<Dbin, D2>)
    explicit constexpr UniquePtr(T2* p, D2&& d) noexcept
        : m_pair(one_then_variadic_args_t{}, std::move(d), p)
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    template<typename T2, typename D2>
        requires SmartPtrConvertible<T, Dbin, T2, D2>
    constexpr UniquePtr(UniquePtr<T2, D2>&& rhs) noexcept
        : m_pair(one_then_variadic_args_t{}, rhs.getDeleter(), rhs.release())
    {
    }

    template<typename T2, typename D2>
        requires SmartPtrConvertible<T, Dbin, T2, D2>
    constexpr UniquePtr& operator=(UniquePtr<T2, D2>&& rhs) noexcept
    {
        reset(rhs.release());
        m_pair.getFirst() = std::forward<D2>(rhs.getDeleter());
        return *this;
    }

    constexpr UniquePtr& operator=(T* p) noexcept
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
    [[nodiscard]] constexpr T* get() const noexcept
    { 
        return m_pair.value;
    }

    /// @brief Release pointer ownership
    /// @return Stored pointer
    constexpr T* release() noexcept
    {
        T* p = m_pair.value;
        m_pair.value = nullptr;
        return p;
    }

    /// @brief Release pointer ownership
    /// @return Stored pointer
    void reset(T* p = nullptr)
    {
        if (m_pair.value == p)
            return;

        if (m_pair.value)
            m_pair.getFirst()(m_pair.value);

        m_pair.value = p;
    }

    template<typename D2>
        requires SmartPtrConvertible<T, Dbin, T, D2>
    void swap(UniquePtr<T, D2>& rhs) noexcept
    {
        T* pTemp = m_pair.value;
        m_pair.value = rhs.m_pair.value;
        rhs.m_pair.value = pTemp;
        auto dTemp = m_pair.getFirst();
        m_pair.getFirst() = rhs.m_pair.getFirst();
        rhs.m_pair.getFirst() = dTemp;
    }

    constexpr Dbin& getDeleter() noexcept
    {
        return m_pair.getFirst();
    }

    constexpr const Dbin& getDeleter() const noexcept
    {
        return m_pair.getFirst();
    }

    [[nodiscard]] constexpr T* operator->() const noexcept
    { 
        return m_pair.value;
    }

    [[nodiscard]] constexpr T& operator*() const noexcept
    { 
        return *m_pair.value;
    }

    constexpr explicit operator bool() const noexcept
    {
        return static_cast<bool>(m_pair.value);
    }

private:
    CompressedPair<Dbin, T*> m_pair;
};

template<typename T, typename Dbin>
class UniquePtr<T[], Dbin>
{
public:
    template<typename D2 = DefaultDeleter<T>>
    UniquePtr()
        : m_pair(zero_then_variadic_args_t{})
    {
    }

    template<typename T2>
        requires SmartPtrArrConvertible<T, Dbin, T2, Dbin>
    explicit constexpr UniquePtr(T2* p) noexcept 
        : m_pair(zero_then_variadic_args_t{}, p)
    {
    }

    template<typename T2, typename D2>
        requires (SmartPtrArrConvertible<T, Dbin, T2, D2> && std::is_constructible_v<Dbin, D2>)
    explicit constexpr UniquePtr(T2* p, const D2& d) noexcept
        : m_pair(one_then_variadic_args_t{}, d, p)
    {
    }

    template<typename T2, typename D2>
        requires (SmartPtrArrConvertible<T, Dbin, T2, D2> && !std::is_reference_v<D2> && std::is_constructible_v<Dbin, D2>)
    explicit constexpr UniquePtr(T2* p, D2&& d) noexcept
        : m_pair(one_then_variadic_args_t{}, std::move(d), p)
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    template<typename T2, typename D2, typename Dbin = DefaultDeleter<T>>
        requires SmartPtrArrConvertible<T, Dbin, T2, D2>
    constexpr UniquePtr(UniquePtr<T2[], D2>&& rhs) noexcept
        : m_pair(one_then_variadic_args_t{}, rhs.getDeleter(), rhs.release())
    {
    }

    template<typename T2, typename D2>
        requires SmartPtrArrConvertible<T, Dbin, T2, D2>
    constexpr UniquePtr& operator=(UniquePtr<T2[], D2>&& rhs) noexcept
    {
        reset(rhs.release());
        m_pair.getFirst() = std::forward<D2>(rhs.getDeleter());
        return *this;
    }

    constexpr UniquePtr& operator=(T* p) noexcept
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
    [[nodiscard]] constexpr T* get() const noexcept
    { 
        return m_pair.value;
    }

    /// @brief Release pointer ownership
    /// @return Stored pointer
    constexpr T* release() noexcept
    {
        T* p = m_pair.value;
        m_pair.value = nullptr;
        return p;
    }

    /// @brief Release pointer ownership
    /// @return Stored pointer
    void reset(T* p = nullptr)
    {
        if (m_pair.value == p)
            return;

        if (m_pair.value)
            m_pair.getFirst()(m_pair.value);

        m_pair.value = p;
    }

    template<typename D2>
        requires SmartPtrConvertible<T, Dbin, T, D2>
    void swap(UniquePtr<T[], D2> & rhs) noexcept
    {
        T* pTemp = m_pair.value;
        m_pair.value = rhs.m_pair.value;
        rhs.m_pair.value = pTemp;
        auto dTemp = m_pair.getFirst();
        m_pair.getFirst() = rhs.m_pair.getFirst();
        rhs.m_pair.getFirst() = dTemp;
    }

    constexpr Dbin& getDeleter() noexcept
    {
        return m_pair.getFirst();
    }

    constexpr const Dbin& getDeleter() const noexcept
    {
        return m_pair.getFirst();
    }

    [[nodiscard]] constexpr T* operator->() const noexcept
    { 
        return m_pair.value;
    }

    [[nodiscard]] constexpr T& operator*() const noexcept
    { 
        return *m_pair.value;
    }

    constexpr explicit operator bool() const noexcept
    {
        return static_cast<bool>(m_pair.value);
    }

private:
    CompressedPair<Dbin, T*> m_pair;
};


template<typename T, typename... Ts>
    requires (!std::is_array_v<T>)
[[nodiscard]] constexpr UniquePtr<T> makeUnique(Ts&&... ts)
{
    return UniquePtr<T>(new T{ std::forward<Ts...>(ts)... });
}

template<typename T, typename... Ts>
    requires (std::is_unbounded_array_v<T>)
[[nodiscard]] constexpr UniquePtr<T> makeUnique(size_t size, Ts&&... ts)
{
    using Type = std::remove_extent_t<T>;
    return UniquePtr<T>(new Type[size]{ std::forward<Ts...>(ts)... });
}

template<typename T, typename... Ts>
    requires (!std::is_array_v<T>)
[[nodiscard]] constexpr UniquePtr<T> makeUniqueNoThrow(Ts&&... ts)
{
    return UniquePtr<T>(new (std::nothrow) T{ std::forward<Ts...>(ts)... });
}

template<typename T, typename... Ts>
    requires (std::is_unbounded_array_v<T>)
[[nodiscard]] constexpr UniquePtr<T> makeUniqueNoThrow(size_t size, Ts&&... ts)
{
    using Type = std::remove_extent_t<T>;
    return UniquePtr<T>(new (std::nothrow) Type[size]{ std::forward<Ts...>(ts)... });
}

template<typename T>
    requires (!std::is_array_v<T>)
[[nodiscard]] constexpr UniquePtr<T> makeUniqueForOverwrite()
{
    return UniquePtr<T>(new T);
}

template<typename T>
    requires (std::is_unbounded_array_v<T>)
[[nodiscard]] constexpr UniquePtr<T> makeUniqueForOverwrite(size_t size)
{
    using Type = std::remove_extent_t<T>;
    return UniquePtr<T>(new Type[size]);
}

template<typename T>
    requires (!std::is_array_v<T>)
[[nodiscard]] constexpr UniquePtr<T> makeUniqueNoThrowForOverwrite()
{
    return UniquePtr<T>(new (std::nothrow) T);
}

template<typename T>
    requires (std::is_unbounded_array_v<T>)
[[nodiscard]] constexpr UniquePtr<T> makeUniqueNoThrowForOverwrite(size_t size)
{
    using Type = std::remove_extent_t<T>;
    return UniquePtr<T>(new (std::nothrow) Type[size]);
}

} // namespace common_serialization
