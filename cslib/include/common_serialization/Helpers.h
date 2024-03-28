/**
 * @file cslib/include/common_serialization/Helpers.h
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

namespace common_serialization
{

struct Dummy {};

/// @brief Get most derived type in CRTP pattern
/// @note If derived type is Dummy then this class is the most derived type.
///     If not - derived type is the most derived type.
/// @tparam X This class type
/// @tparam T Derived type
template<typename X, typename T>
using GetCrtpMainType = std::conditional_t<std::is_same_v<T, common_serialization::Dummy>, X, T>;

template<typename T>
struct remove_member_pointer
{
    using type = T;
};

template<typename C, typename T>
struct remove_member_pointer<T C::*>
{
    using type = T;
};

template<typename T>
using remove_member_pointer_t = typename remove_member_pointer<T>::type;

template<typename T, std::size_t L>
struct pointer_level_traits_impl
{
    static const std::size_t value = L;
    using from_ptr_to_const_to_ptr = std::remove_const_t<T>;
};

template<typename T, std::size_t L>
    requires std::is_pointer_v<T>
struct pointer_level_traits_impl<T, L> : pointer_level_traits_impl<std::remove_pointer_t<T>, L + 1>
{
    using from_ptr_to_const_to_ptr = typename pointer_level_traits_impl<std::remove_pointer_t<T>, L + 1>::from_ptr_to_const_to_ptr*;
};

template<typename T>
    requires std::is_pointer_v<T>
struct pointer_level_traits : pointer_level_traits_impl<std::remove_pointer_t<T>, 0>
{
    using from_ptr_to_const_to_ptr = typename pointer_level_traits_impl<std::remove_pointer_t<T>, 0>::from_ptr_to_const_to_ptr*;
};

/// @brief Erases all consts from pointer
/// @note Works only with pointers
/// @remark const int* const * const * const -> int***
template<typename T>
    requires std::is_pointer_v<T>
using from_ptr_to_const_to_ptr_t = typename pointer_level_traits<T>::from_ptr_to_const_to_ptr;

template<typename T>
concept IsNotPointer = !(std::is_pointer_v<T> || std::is_member_pointer_v<T> || std::is_function_v<T> || std::is_member_function_pointer_v<T>);

namespace helpers
{

/// @brief Is current module compiled for 32 bitness
/// @return True if 32 bitness, false otherwise
consteval bool isBitness32()
{
    return
#ifndef X32
        false;
#else
        true;
#endif
}

/// @brief Is current module compiled with big-endian format
/// @return True if big-endian, false if little-endian
consteval bool isBigEndianPlatform()
{
    return
#ifndef BIG_ENDIAN_PLATFORM
        false;
#else
        true;
#endif
}

/// @brief Is current module compiled with little-endian format
/// @return True if little-endian, false if big-endian
consteval bool isLittleEndianPlatform()
{
    return !isBigEndianPlatform();
}

/// @brief Test for overlapping memory regions of same sized arrays
/// @tparam T Type of arrays
/// @param objs1 Start of first array
/// @param objs2 Start of second array
/// @param n Number of elements in each array
/// @return True if there is overlap of memory regions
template<typename T>
constexpr bool areRegionsOverlap(const T* pObjs1, const T* pObjs2, size_t n)
{
    return pObjs1 >= pObjs2 && pObjs1 < pObjs2 + n || pObjs1 <= pObjs2 && pObjs1 + n > pObjs2;
}

/// @brief Get array size
/// @tparam T Array element type
/// @tparam N Size of array
/// @param arr Array which size we need to get
/// @return Size of Array
template<typename T, size_t N>
consteval size_t countof(T(&arr)[N])
{
    return N;
}

/// @brief Convert uint64_t endianness from little-endian to big-endian and vice versa
/// @param input Number
/// @return Converted number
constexpr uint64_t reverseEndianessUint64(uint64_t input)
{
    return input >> 56
        | (input >> 40 & 0x000000000000ff00)
        | (input >> 24 & 0x0000000000ff0000)
        | (input >> 8 & 0x00000000ff000000)
        | (input << 8 & 0x000000ff00000000)
        | (input << 24 & 0x0000ff0000000000)
        | (input << 40 & 0x00ff000000000000)
        | input << 56;
}

/// @brief Convert uint32_t endianness from little-endian to big-endian and vice versa
/// @param input Number
/// @return Converted number
constexpr uint32_t reverseEndianessUint32(uint32_t input)
{
    return input >> 24
        | (input >> 8 & 0x0000ff00)
        | (input << 8 & 0x00ff0000)
        | input << 24;
}

/// @brief Convert uint16_t endianness from little-endian to big-endian and vice versa
/// @param input Number
/// @return Converted number
constexpr uint16_t reverseEndianessUint16(uint16_t input)
{
    return input >> 8 | input << 8;
}

} // namespace helpers

} // namespace common_serialization
