/**
 * @file common_serialization/common/Uuid.h
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
#include <common_serialization/common/helpers.h>

namespace common_serialization
{

#pragma pack(push, 1)

/// @brief Struct for holding UUIDv4
struct Uuid
{
    using always_simply_assignable_tag = std::true_type;
    using endianness_tolerant_tag = std::true_type;

    // Note that UUID is placed in BigEndian order, 
    uint64_t m_high{ 0 };
    uint64_t m_low{ 0 };

    constexpr Uuid() = default;

    constexpr Uuid(uint64_t high, uint64_t low)
    {
        if constexpr (helpers::isLittleEndianPlatform())
        {
            m_high = helpers::reverseEndianess(high);
            m_low = helpers::reverseEndianess(low);
        }
        else
        {
            m_high = high;
            m_low = low;
        }
    }

    constexpr Uuid(uint32_t first, uint16_t second, uint16_t third, uint16_t fourth, uint64_t fifth) noexcept
    {
        if constexpr (helpers::isLittleEndianPlatform())
        {
            m_high = helpers::reverseEndianess(first)
                | static_cast<uint64_t>(helpers::reverseEndianess(second)) << 32 
                | static_cast<uint64_t>(helpers::reverseEndianess(third)) << 48;
            m_low = helpers::reverseEndianess(fourth) | (helpers::reverseEndianess(fifth) & 0xffffffffffff0000);
        }
        else
        {
            m_high = static_cast<uint64_t>(first) << 32 | static_cast<uint64_t>(second) << 16 | third;
            m_low = static_cast<uint64_t>(fourth) << 48 | fifth;
        }
    }
   
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] bool operator<(const Uuid& rhs) const noexcept
    {
        if constexpr (helpers::isLittleEndianPlatform())
            return m_low < rhs.m_low || m_low == rhs.m_low && m_high < rhs.m_high;
        else
            return m_high < rhs.m_high || m_high == rhs.m_high && m_low < rhs.m_low;
    }

    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] bool operator==(const Uuid& rhs) const noexcept
    {
        return m_low == rhs.m_low && m_high == rhs.m_high;
    }
};

#pragma pack(pop)

constexpr Uuid kNullUuid{};

} // namespace common_serialization

template<>
struct std::hash<common_serialization::Uuid>
{
    std::size_t operator()(const common_serialization::Uuid& id) const noexcept
    {
        auto hashFunc = std::hash<uint8_t>{};
        const uint8_t* pIdBytes = reinterpret_cast<const uint8_t*>(&id);
        size_t result{ 0 };
        for (size_t i = 0; i < 16; ++i)
            result ^= hashFunc(pIdBytes[i]);

        return result;
    }
};
