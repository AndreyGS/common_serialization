/**
 * @file common_serialization/common/std_equivalents.h
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

 // if implementation wants to define own std shadowing functions
 // it should define CS_NO_STD_META_FUNCS_CUSTOM_DEFINITION macro
#if !defined CS_NO_STD_META_FUNCS_CUSTOM_DEFINITION && (defined WINDOWS_KERNEL || defined LINUX_KERNEL)

namespace std
{

// std::nullptr_t
using nullptr_t = decltype(nullptr);

// std::nothrow
struct nothrow_t { };
inline constexpr nothrow_t nothrow;

// std::integral_constant<_T, v>
template<typename _T, _T v>
struct integral_constant
{
    static constexpr _T value = v;

    using value_type = _T;
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

template<typename _T>
inline constexpr bool is_same_v<_T, _T> = true;

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
template<typename _T>
struct remove_reference
{
    using type = _T;
};

template<typename _T>
struct remove_reference<_T&>
{
    using type = _T;
};

template<typename _T>
struct remove_reference<_T&&>
{
    using type = _T;
};

// std::remove_reference_t<_T>
template<typename _T>
using remove_reference_t = typename remove_reference<_T>::type;

// std::is_lvalue_reference_v<_T>
template<typename>
inline constexpr bool is_lvalue_reference_v = false;

template<typename _T>
inline constexpr bool is_lvalue_reference_v<_T&> = true;

template<typename _T>
struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<_T>>
{
};

// std::move
template<typename _T>
[[nodiscard]] constexpr remove_reference_t<_T>&& move(_T&& t) noexcept
{
    return static_cast<remove_reference_t<_T>&&>(t);
}

// std::forward
template<typename _T>
[[nodiscard]] constexpr _T&& forward(remove_reference_t<_T>& t) noexcept
{
    return static_cast<_T&&>(t);
}

template <class _T>
[[nodiscard]] constexpr _T&& forward(remove_reference_t<_T>&& t) noexcept
{
    static_assert(!is_lvalue_reference_v<_T>, "illegal use of std::forward");
    return static_cast<_T&&>(t);
}

// std::remove_volatile
template<typename _T>
struct remove_volatile
{
    using type = _T;
};

template<typename _T>
struct remove_volatile<volatile _T>
{
    using type = _T;
};

// std::remove_volatile_t
template<typename _T>
using remove_volatile_t = typename remove_volatile<_T>::type;

// std::remove_const
template<typename _T>
struct remove_const
{
    using type = _T;
};

template<typename _T>
struct remove_const<const _T>
{
    using type = _T;
};

// std::remove_const_t
template<typename _T>
using remove_const_t = typename remove_const<_T>::type;

// std::remove_cv
template<typename _T>
struct remove_cv
{
    using type = _T;
};

template<typename _T>
struct remove_cv<const _T>
{
    using type = _T;
};

template<typename _T>
struct remove_cv<volatile _T>
{
    using type = _T;
};

template<typename _T>
struct remove_cv<const volatile _T>
{
    using type = _T;
};

// std::remove_cv_t
template<typename _T>
using remove_cv_t = typename remove_cv<_T>::type;

// std::is_integral_v
template<typename _T>
inline constexpr bool is_integral_v = is_same_v<remove_cv_t<_T>, bool> || is_same_v<remove_cv_t<_T>, char> || is_same_v<remove_cv_t<_T>, signed char>
    || is_same_v<remove_cv_t<_T>, unsigned char> || is_same_v<remove_cv_t<_T>, short> || is_same_v<remove_cv_t<_T>, unsigned short> || is_same_v<remove_cv_t<_T>, int>
    || is_same_v<remove_cv_t<_T>, unsigned int> || is_same_v<remove_cv_t<_T>, long> || is_same_v<remove_cv_t<_T>, unsigned long> || is_same_v<remove_cv_t<_T>, long long>
    || is_same_v<remove_cv_t<_T>, unsigned long long> || is_same_v<remove_cv_t<_T>, wchar_t> || is_same_v<remove_cv_t<_T>, char8_t> || is_same_v<remove_cv_t<_T>, char16_t>
    || is_same_v<remove_cv_t<_T>, char32_t>;

template<typename _T>
inline constexpr bool is_floating_point_v = is_same_v<remove_cv_t<_T>, float> || is_same_v<remove_cv_t<_T>, double> || is_same_v<remove_cv_t<_T>, long double>;

template<typename _T>
inline constexpr bool is_arithmetic_v = is_integral_v<_T> || is_floating_point_v<_T>;

// std::remove_pointer
template<typename _T>
struct remove_pointer
{
    using type = _T;
};

template<typename _T>
struct remove_pointer<_T*>
{
    using type = _T;
};

template<typename _T>
struct remove_pointer<_T* const>
{
    using type = _T;
};

template<typename _T>
struct remove_pointer<_T* volatile>
{
    using type = _T;
};

template<typename _T>
struct remove_pointer<_T* const volatile>
{
    using type = _T;
};

// std::remove_pointer_t
template<typename _T>
using remove_pointer_t = typename remove_pointer<_T>::type;

// std::is_array
template<typename _T>
struct is_array : std::false_type {};

template<typename _T>
struct is_array<_T[]> : std::true_type {};

template<typename _T, size_t N>
struct is_array<_T[N]> : std::true_type {};

// std::is_array_v
template<typename _T>
inline constexpr bool is_array_v = is_array<_T>::value;

// std::is_bounded_array
template<typename _T>
struct is_bounded_array : std::false_type {};

template<typename _T, size_t N>
struct is_bounded_array<_T[N]> : std::true_type {};

// std::is_bounded_array_v
template<typename _T>
inline constexpr bool is_bounded_array_v = is_bounded_array<_T>::value;

// std::is_unbounded_array
template<typename _T>
struct is_unbounded_array : std::false_type {};

template<typename _T>
struct is_unbounded_array<_T[]> : std::true_type {};

// std::is_unbounded_array_v
template<typename _T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<_T>::value;

// std::remove_extent
template <typename _T>
struct remove_extent
{
    using type = _T;
};

template <typename _T, size_t N>
struct remove_extent<_T[N]>
{
    using type = _T;
};

template <typename _T>
struct remove_extent<_T[]>
{
    using type = _T;
};

// std::remove_extent_t
template <typename _T>
using remove_extent_t = typename remove_extent<_T>::type;

// std::is_reference
template<typename _T>
struct is_reference : std::false_type {};

template<typename _T>
struct is_reference<_T&> : std::true_type {};

template<typename _T>
struct is_reference<_T&&> : std::true_type {};

// std::is_reference_v
template<typename _T>
inline constexpr bool is_reference_v = is_reference<_T>::value;

// std::is_const
template<typename _T>
struct is_const : std::false_type {};

template<typename _T>
struct is_const<const _T> : std::true_type {};

// std::is_const_v
template<typename _T>
inline constexpr bool is_const_v = is_const<_T>::value;

// std::is_function
template<typename _T>
struct is_function : std::integral_constant<bool, !std::is_const<const _T>::value && !std::is_reference<_T>::value> {};

// std::is_function_v
template<typename _T>
inline constexpr bool is_function_v = is_function<_T>::value;

// std::is_array
template<class _T>
struct is_array : std::false_type {};

template<class _T>
struct is_array<_T[]> : std::true_type {};

template<class _T, std::size_t N>
struct is_array<_T[N]> : std::true_type {};

template< class _T >
inline constexpr bool is_array_v = is_array<_T>::value;

// std::is_pointer
template<typename _T>
struct is_pointer : std::false_type {};

template<typename _T>
struct is_pointer<_T*> : std::true_type {};

template<typename _T>
struct is_pointer<_T* const> : std::true_type {};

template<typename _T>
struct is_pointer<_T* volatile> : std::true_type {};

template<typename _T>
struct is_pointer<_T* const volatile> : std::true_type {};

// std::is_pointer_v
template<typename _T>
inline constexpr bool is_pointer_v = is_pointer<_T>::value;

// std::is_member_pointer
template<typename _T>
struct is_member_pointer_helper : std::false_type {};

template<typename _T, typename U>
struct is_member_pointer_helper<_T U::*> : std::true_type {};

template<typename _T>
struct is_member_pointer : is_member_pointer_helper<std::remove_cv_t<_T>> {};

// std::is_member_pointer_v
template<typename _T>
inline constexpr bool is_member_pointer_v = is_member_pointer<_T>::value;

// std::is_member_function_pointer
template<typename _T>
struct is_member_function_pointer_helper : std::false_type {};

template<typename _T, typename U>
struct is_member_function_pointer_helper<_T U::*> : std::is_function<_T> {};

template<typename _T>
struct is_member_function_pointer : is_member_function_pointer_helper<std::remove_cv_t<_T>> {};

// std::is_member_function_pointer_v
template<typename _T>
inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<_T>::value;

// std::is_enum
template<typename _T>
struct is_enum : bool_constant<__is_enum(_T)> {};

// std::is_enum_v
template<typename _T>
inline constexpr bool is_enum_v = __is_enum(_T);

// std::is_base_of
template<typename _Base, typename _Derived>
struct is_base_of : bool_constant<__is_base_of(_Base, _Derived)> {};

// std::is_base_of_v
template<typename _Base, typename _Derived>
inline constexpr bool is_base_of_v = __is_base_of(_Base, _Derived);

// std::is_union
template<typename _T>
struct is_union : bool_constant<__is_union(_T)> {};

// std::is_union_v
template<typename _T>
inline constexpr bool is_union_v = __is_union(_T);

// std::is_class
template<typename _T>
struct is_class : bool_constant<__is_class(_T)> {};

// std::is_class_v
template<typename _T>
inline constexpr bool is_class_v = __is_class(_T);

// std::conditional
template<bool B, typename _T, typename F>
struct conditional { using type = _T; };

template<typename _T, typename F>
struct conditional<false, _T, F>
{
    using type = F;
};

// std::conditional_t
template<bool B, class _T, class F>
using conditional_t = typename conditional<B, _T, F>::type;

// std::underlying_type
template<typename _T, bool = is_enum_v<_T>>
struct underlying_type
{
    using type = __underlying_type(_T);
};

template<typename _T>
struct underlying_type<_T, false> {};

// std::underlying_type_t
template<typename _T>
using underlying_type_t = typename underlying_type<_T>::type;

// std::is_trivially_copyable
template<typename _T>
struct is_trivially_copyable : bool_constant<__is_trivially_copyable(_T)> {};

// std::is_trivially_copyable_v
template<typename _T>
inline constexpr bool is_trivially_copyable_v = __is_trivially_copyable(_T);

// std::is_trivial
template<typename _T>
struct is_trivial : bool_constant<__is_trivially_constructible(_T) && __is_trivially_copyable(_T)> {};

// std::is_trivial_v
template<typename _T>
inline constexpr bool is_trivial_v = __is_trivially_constructible(_T) && __is_trivially_copyable(_T);

// std::size
template <typename _T, size_t N>
size_t size(_T(&)[N])
{
    return N;
}

template <bool _Test, class _T1, class _T2>
struct conditional
{
    using type = _T1;
};

template <class _T1, class _T2>
struct conditional<false, _T1, _T2>
{
    using type = _T2;
};

template<bool _Test, class _T1, class _T2>
using conditional_t = typename conditional<_Test, _T1, _T2>::type;

enum class endian
{
#if defined(_MSC_VER) && !defined(__clang__)
    little = 0,
    big    = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};

// std::is_convertible
template<typename _From, typename _To>
struct is_convertible : bool_constant<__is_convertible_to(_From, _To)>
{
};

// std::is_convertible_v
template<typename _From, typename _To>
inline constexpr bool is_convertible_v = __is_convertible_to(_From, _To);

// std::is_empty
template<typename _T>
struct is_empty : bool_constant<__is_empty(_T)> {};

// std::is_empty_v
template<typename _T>
inline constexpr bool is_empty_v = __is_empty(_T);

// std::is_final
template<typename _T>
struct is_final : bool_constant<__is_final(_T)> {};

// std::is_final_v
template<typename _T>
inline constexpr bool is_final_v = __is_final(_T);

// std::has_virtual_destructor
template<typename _T>
struct has_virtual_destructor : bool_constant<__has_virtual_destructor(_T)>
{
};

// std::has_virtual_destructor_v
template<typename _T>
inline constexpr bool has_virtual_destructor_v = __has_virtual_destructor(_T);

// std::destroying_delete_t
struct destroying_delete_t {};

// std::is_constructible
template<typename _T, typename... Ts>
struct is_constructible : bool_constant<__is_constructible(_T, Ts...)> 
{
};

// std::is_constructible_v
template<typename _T, typename... Ts>
inline constexpr bool is_constructible_v = __is_constructible(_T, Ts...);

} // namespace std

#endif // #ifndef CS_NO_STD_META_FUNCS_CUSTOM_DEFINITION
