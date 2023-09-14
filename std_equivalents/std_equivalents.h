/**
 * @file std_equivalents.h
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

namespace std
{

// std::nothrow
struct nothrow_t { };
inline constexpr nothrow_t nothrow;

// std::integral_constant<T, v>
template<typename T, T v>
struct integral_constant
{
    static constexpr T value = v;

    using value_type = T;
    using type = integral_constant;

    constexpr operator value_type() const noexcept
    {
        return value;
    }

    [[nodiscard]] constexpr value_type operator()() const noexcept
    {
        return value;
    }
};

// std::bool_constant<_Val>
template <bool _Val>
using bool_constant = integral_constant<bool, _Val>;

// std::is_same_v
template<typename, typename>
inline constexpr bool is_same_v = false;

template<typename T>
inline constexpr bool is_same_v<T, T> = true;

// std::is_same<X, Y>
template<typename X, typename Y>
struct is_same : bool_constant<is_same_v<X, Y>>
{
};

// std::same_as<X, Y>
template<typename X, typename Y>
concept same_as = is_same_v<X, Y> && is_same_v<Y, X>;

// std::true_type
using true_type = bool_constant<true>;

// std::false_type
using false_type = bool_constant<false>;


// std::remove_reference
template<typename T>
struct remove_reference
{
    using type = T;
};

template<typename T>
struct remove_reference<T&>
{
    using type = T;
};

template<typename T>
struct remove_reference<T&&>
{
    using type = T;
};

// std::remove_reference_t<T>
template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

// std::move
template<typename T>
[[nodiscard]] constexpr remove_reference_t<T>&& move(T&& t) noexcept
{
    return static_cast<remove_reference_t<T>&&>(t);
}

// std::is_lvalue_reference_v<T>
template<typename>
inline constexpr bool is_lvalue_reference_v = false;

template<typename T>
inline constexpr bool is_lvalue_reference_v<T&> = true;

template<typename T>
struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<T>>
{
};


// std::remove_volatile
template<typename T>
struct remove_volatile
{
    using type = T;
};

template<typename T>
struct remove_volatile<volatile T>
{
    using type = T;
};

// std::remove_volatile_t
template<typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

// std::remove_const
template<typename T>
struct remove_const
{
    using type = T;
};

template<typename T>
struct remove_const<const T>
{
    using type = T;
};

// std::remove_const_t
template<typename T>
using remove_const_t = typename remove_const<T>::type;

// std::remove_cv
template<typename T>
struct remove_cv
{
    using type = T;
};

template<typename T>
struct remove_cv<const T>
{
    using type = T;
};

template<typename T>
struct remove_cv<volatile T>
{
    using type = T;
};

template<typename T>
struct remove_cv<const volatile T>
{
    using type = T;
};

// std::remove_cv_t
template<typename T>
using remove_cv_t = typename remove_cv<T>::type;

// std::is_integral_v
template<typename T>
inline constexpr bool is_integral_v = is_same_v<remove_cv_t<T>, bool> || is_same_v<remove_cv_t<T>, char> || is_same_v<remove_cv_t<T>, signed char>
    || is_same_v<remove_cv_t<T>, unsigned char> || is_same_v<remove_cv_t<T>, short> || is_same_v<remove_cv_t<T>, unsigned short> || is_same_v<remove_cv_t<T>, int>
    || is_same_v<remove_cv_t<T>, unsigned int> || is_same_v<remove_cv_t<T>, long> || is_same_v<remove_cv_t<T>, unsigned long> || is_same_v<remove_cv_t<T>, long long>
    || is_same_v<remove_cv_t<T>, unsigned long long> || is_same_v<remove_cv_t<T>, wchar_t> || is_same_v<remove_cv_t<T>, char8_t> || is_same_v<remove_cv_t<T>, char16_t>
    || is_same_v<remove_cv_t<T>, char32_t>;

template<typename T>
inline constexpr bool is_floating_point_v = is_same_v<remove_cv_t<T>, float> || is_same_v<remove_cv_t<T>, double> || is_same_v<remove_cv_t<T>, long double>;

template<typename T>
inline constexpr bool is_arithmetic_v = is_integral_v<T> || is_floating_point_v<T>;

// std::remove_pointer
template<typename T>
struct remove_pointer
{
    using type = _Ty;
};

template<typename T>
struct remove_pointer<T*>
{
    using type = T;
};

template<typename T>
struct remove_pointer<T* const>
{
    using type = T;
};

template<typename T>
struct remove_pointer<T* volatile>
{
    using type = T;
};

template<typename T>
struct remove_pointer<T* const volatile>
{
    using type = T;
};

// std::remove_pointer_t
template<typename T>
using remove_pointer_t = typename remove_pointer<T>::type;


// std::forward
template<typename T>
[[nodiscard]] constexpr T&& forward(remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template <class T>
[[nodiscard]] constexpr T&& forward(remove_reference_t<T>&& t) noexcept
{
    static_assert(!is_lvalue_reference_v<T>, "illegal use of std::forward");
    return static_cast<T&&>(t);
}

// std::nothrow
struct nothrow_t { };
inline constexpr nothrow_t nothrow;

} // namespace std
