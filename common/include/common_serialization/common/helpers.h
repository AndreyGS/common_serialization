/**
 * @file common_serialization/common/helpers.h
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

#include <common_serialization/common/traits.h>

namespace common_serialization::helpers
{

/// @brief Is current module compiled for 32 bitness
/// @return True if 32 bitness, false otherwise
consteval bool isBitness32()
{
    return
#ifndef X32_PLATFORM
        false;
#else
        true;
#endif
}

/// @brief Is current module compiled for 64 bitness
/// @return True if 64 bitness, false otherwise
consteval bool isBitness64()
{
    return !isBitness32();
}

/// @brief Is current module compiled with big-endian format
/// @return True if big-endian, false if little-endian
consteval bool isBigEndianPlatform()
{
    return std::endian::native == std::endian::big;
}

/// @brief Is current module compiled with little-endian format
/// @return True if little-endian, false if big-endian
consteval bool isLittleEndianPlatform()
{
    return !isBigEndianPlatform();
}

/// @brief Test for overlapping memory regions of same sized arrays
/// @tparam _T Type of arrays
/// @param objs1 Start of first array
/// @param objs2 Start of second array
/// @param n Number of elements in each array
/// @return True if there is overlap of memory regions
template<typename _T>
constexpr bool areRegionsOverlap(const _T* pObjs1, const _T* pObjs2, size_t n)
{
    return pObjs1 >= pObjs2 && pObjs1 < pObjs2 + n || pObjs1 <= pObjs2 && pObjs1 + n > pObjs2;
}

/// @brief Get array size
/// @tparam _T Array element type
/// @tparam N Size of array
/// @param arr Array which size we need to get
/// @return Size of Array
template<typename _T, size_t N>
consteval size_t countof(_T(&arr)[N])
{
    return N;
}

/// @brief Convert uint64_t endianness from little-endian to big-endian and vice versa
/// @param input Number
/// @return Converted number
constexpr uint64_t reverseEndianessUInt64(uint64_t input)
{
    return input >> 56
        | (input >> 40 & 0x000000000000ff00)
        | (input >> 24 & 0x0000000000ff0000)
        | (input >>  8 & 0x00000000ff000000)
        | (input <<  8 & 0x000000ff00000000)
        | (input << 24 & 0x0000ff0000000000)
        | (input << 40 & 0x00ff000000000000)
        |  input << 56;
}

/// @brief Convert uint32_t endianness from little-endian to big-endian and vice versa
/// @param input Number
/// @return Converted number
constexpr uint32_t reverseEndianessUInt32(uint32_t input)
{
    return input >> 24
        | (input >>  8 & 0x0000ff00)
        | (input <<  8 & 0x00ff0000)
        |  input << 24;
}

/// @brief Convert uint16_t endianness from little-endian to big-endian and vice versa
/// @param input Number
/// @return Converted number
constexpr uint16_t reverseEndianessUInt16(uint16_t input)
{
    return input >> 8 | input << 8;
}

template<typename _T>
    requires EndiannessReversable<_T>
AGS_CS_ALWAYS_INLINE constexpr _T reverseEndianess(_T input)
{
    if constexpr (sizeof(_T) == 2)
    {
        if constexpr (std::is_integral_v<_T>)
            return reverseEndianessUInt16(input);
        else
        {
            uint16_t temp = reverseEndianessUInt16(*static_cast<uint16_t*>(static_cast<void*>(&input)));
            return *static_cast<_T*>(static_cast<void*>(&temp));
        }
    }
    else if constexpr (sizeof(_T) == 4)
    {
        
        if constexpr (std::is_integral_v<_T>)
            return reverseEndianessUInt32(input);
        else
        {
            uint32_t temp = reverseEndianessUInt32(*static_cast<uint32_t*>(static_cast<void*>(&input)));
            return *static_cast<_T*>(static_cast<void*>(&temp));
        }
    }
    else
    {
        static_assert(sizeof(_T) == 8);

        if constexpr (std::is_integral_v<_T>)
            return reverseEndianessUInt64(input);
        else
        {
            uint64_t temp = reverseEndianessUInt64(*static_cast<uint64_t*>(static_cast<void*>(&input)));
            return *static_cast<_T*>(static_cast<void*>(&temp));
        }
    }
}

template<size_t targetSize, bool isSigned>
struct fixed_width_integer;

template<>
struct fixed_width_integer<8, true>
{
    using type = int64_t;
    static constexpr type min = -9223372036854775807LL - 1;
    static constexpr type max = 9223372036854775807LL;
};

template<>
struct fixed_width_integer<8, false>
{
    using type = uint64_t;
    static constexpr type min = 0ULL;
    static constexpr type max = 18446744073709551615ULL;
};

template<>
struct fixed_width_integer<4, true>
{
    using type = int32_t;
    static constexpr type min = -2147483647 - 1;
    static constexpr type max = 2147483647;
};

template<>
struct fixed_width_integer<4, false>
{
    using type = uint32_t;
    static constexpr type min = 0;
    static constexpr type max = 4294967295U;
};

template<>
struct fixed_width_integer<2, true>
{
    using type = int16_t;
    static constexpr type min = -32767 - 1;
    static constexpr type max = 32767;
};

template<>
struct fixed_width_integer<2, false>
{
    using type = uint16_t;
    static constexpr type min = 0;
    static constexpr type max = 65535;
};

template<>
struct fixed_width_integer<1, true>
{
    using type = int8_t;
    static constexpr type min = -127 - 1;
    static constexpr type max = 127;
};

template<>
struct fixed_width_integer<1, false>
{
    using type = uint8_t;
    static constexpr type min = 0;
    static constexpr type max = 255;
};

template<size_t targetSize, bool isSigned>
using fixed_width_integer_t = typename fixed_width_integer<targetSize, isSigned>::type;

template<typename _T1, typename _T2>
    requires (std::is_integral_v<_T1> && sizeof(_T2) <= sizeof(_T1) && std::is_same_v<_T2, fixed_width_integer_t<sizeof(_T2), Signed<_T2>>>)
constexpr Status castToSmallerType(_T1 input, _T2& output)
{
    using output_type = fixed_width_integer<sizeof(_T2), Signed<_T2>>;

    if (input > output_type::max || input < output_type::min)
        return Status::ErrorValueOverflow;

    output = static_cast<typename output_type::type>(input);

    return Status::NoError;
}

template<typename _T1, typename _T2>
    requires (std::is_integral_v<_T1> && sizeof(_T2) >= sizeof(_T1) && std::is_same_v<_T2, fixed_width_integer_t<sizeof(_T2), Signed<_T2>>>)
AGS_CS_ALWAYS_INLINE constexpr void castToBiggerType(_T1 input, _T2& output)
{
    output = static_cast<fixed_width_integer_t<sizeof(_T2), Signed<_T2>>>(input);
}

} // namespace common_serialization::helpers
