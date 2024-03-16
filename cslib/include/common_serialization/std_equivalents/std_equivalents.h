/**
 * @file cslib/include/common_serialization/std_equivalents/std_equivalents.h
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

using int_fast32_t = int;

namespace std
{

// std::nullptr_t
using nullptr_t = decltype(nullptr);

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
struct is_same : bool_constant<is_same_v<X, Y>> {};

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

// std::is_lvalue_reference_v<T>
template<typename>
inline constexpr bool is_lvalue_reference_v = false;

template<typename T>
inline constexpr bool is_lvalue_reference_v<T&> = true;

template<typename T>
struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<T>>
{
};

// std::move
template<typename T>
[[nodiscard]] constexpr remove_reference_t<T>&& move(T&& t) noexcept
{
    return static_cast<remove_reference_t<T>&&>(t);
}

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
    using type = T;
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

// std::is_array
template<typename T>
struct is_array : std::false_type {};

template<typename T>
struct is_array<T[]> : std::true_type {};

template<typename T, size_t N>
struct is_array<T[N]> : std::true_type {};

// std::is_array_v
template<typename T>
inline constexpr bool is_array_v = is_array<T>::value;

// std::is_bounded_array
template<typename T>
struct is_bounded_array : std::false_type {};

template<typename T, size_t N>
struct is_bounded_array<T[N]> : std::true_type {};

// std::is_bounded_array_v
template<typename T>
inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

// std::is_unbounded_array
template<typename T>
struct is_unbounded_array : std::false_type {};

template<typename T>
struct is_unbounded_array<T[]> : std::true_type {};

// std::is_unbounded_array_v
template<typename T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

// std::is_reference
template<typename T>
struct is_reference : std::false_type {};

template<typename T>
struct is_reference<T&> : std::true_type {};

template<typename T>
struct is_reference<T&&> : std::true_type {};

// std::is_reference_v
template<typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;

// std::is_const
template<typename T>
struct is_const : std::false_type {};

template<typename T>
struct is_const<const T> : std::true_type {};

// std::is_const_v
template<typename T>
inline constexpr bool is_const_v = is_const<T>::value;

// std::is_function
template<typename T>
struct is_function : std::integral_constant<bool, !std::is_const<const T>::value && !std::is_reference<T>::value> {};

// std::is_function_v
template<typename T>
inline constexpr bool is_function_v = is_function<T>::value;

// std::is_array
template<class T>
struct is_array : std::false_type {};

template<class T>
struct is_array<T[]> : std::true_type {};

template<class T, std::size_t N>
struct is_array<T[N]> : std::true_type {};

template< class T >
inline constexpr bool is_array_v = is_array<T>::value;

// std::is_pointer
template<typename T>
struct is_pointer : std::false_type {};

template<typename T>
struct is_pointer<T*> : std::true_type {};

template<typename T>
struct is_pointer<T* const> : std::true_type {};

template<typename T>
struct is_pointer<T* volatile> : std::true_type {};

template<typename T>
struct is_pointer<T* const volatile> : std::true_type {};

// std::is_pointer_v
template<typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

// std::is_member_pointer
template<typename T>
struct is_member_pointer_helper : std::false_type {};

template<typename T, typename U>
struct is_member_pointer_helper<T U::*> : std::true_type {};

template<typename T>
struct is_member_pointer : is_member_pointer_helper<std::remove_cv_t<T>> {};

// std::is_member_pointer_v
template<typename T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

// std::is_member_function_pointer
template<typename T>
struct is_member_function_pointer_helper : std::false_type {};

template<typename T, typename U>
struct is_member_function_pointer_helper<T U::*> : std::is_function<T> {};

template<typename T>
struct is_member_function_pointer : is_member_function_pointer_helper<std::remove_cv_t<T>> {};

// std::is_member_function_pointer_v
template<typename T>
inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

// std::is_enum
template<typename T>
struct is_enum : bool_constant<__is_enum(T)> {};

// std::is_enum_v
template<typename T>
inline constexpr bool is_enum_v = __is_enum(T);

// std::is_base_of
template<typename Base, typename Derived>
struct is_base_of : bool_constant<__is_base_of(Base, Derived)> {};

// std::is_base_of_v
template<typename Base, typename Derived>
inline constexpr bool is_base_of_v = __is_base_of(Base, Derived);

// std::is_union
template<typename T>
struct is_union : bool_constant<__is_union(T)> {};

// std::is_union_v
template<typename T>
inline constexpr bool is_union_v = __is_union(T);

// std::is_class
template<typename T>
struct is_class : bool_constant<__is_class(T)> {};

// std::is_class_v
template<typename T>
inline constexpr bool is_class_v = __is_class(T);

// std::conditional
template<bool B, typename T, typename F>
struct conditional { using type = T; };

template<typename T, typename F>
struct conditional<false, T, F>
{
    using type = F;
};

// std::conditional_t
template<bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

// std::underlying_type
template<typename T, bool = is_enum_v<T>>
struct underlying_type
{
    using type = __underlying_type(T);
};

template<typename T>
struct underlying_type<T, false> {};

// std::underlying_type_t
template<typename T>
using underlying_type_t = typename underlying_type<T>::type;

// std::is_trivially_copyable
template<typename T>
struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};

// std::is_trivially_copyable_v
template<typename T>
inline constexpr bool is_trivially_copyable_v = __is_trivially_copyable(T);

// std::is_trivial
template<typename T>
struct is_trivial : bool_constant<__is_trivially_constructible(T) && __is_trivially_copyable(T)> {};

// std::is_trivial_v
template<typename T>
inline constexpr bool is_trivial_v = __is_trivially_constructible(T) && __is_trivially_copyable(T);

// std::size
template <typename T, std::size_t N>
std::size_t size(T(&)[N])
{
    return N;
}

template <bool Test, class T1, class T2>
struct conditional
{
    using type = T1;
};

template <class T1, class T2>
struct conditional<false, T1, T2>
{
    using type = T2;
};

template<bool Test, class T1, class T2>
using conditional_t = typename conditional<Test, T1, T2>::type;

} // namespace std
