/**
 * @file cslib/include/common_serialization/Uuid.h
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

    union
    {
        uint8_t m_id[16] { 0 };

        // Note that UUID is placed in BigEndian order, 
        // so when using 'm_high' and/or 'm_low' logic or numbers itself must be reversed.
        struct
        {
            uint64_t m_high;
            uint64_t m_low;
        };
    };

    constexpr Uuid() = default;
    constexpr Uuid(const Uuid& rhs) = default;

    constexpr Uuid(uint32_t first, uint16_t second, uint16_t third, uint16_t fourth, uint64_t fifth) noexcept
    {
        m_id[0] = (first & 0xff000000) >> 24;
        m_id[1] = (first & 0x00ff0000) >> 16;
        m_id[2] = (first & 0x0000ff00) >> 8;
        m_id[3] = first & 0x000000ff;
        m_id[4] = (second & 0xff00) >> 8;
        m_id[5] = second & 0x00ff;
        m_id[6] = (third & 0xff00) >> 8;
        m_id[7] = third & 0x00ff;
        m_id[8] = (fourth & 0xff00) >> 8;
        m_id[9] = fourth & 0x00ff;
        m_id[10] = (fifth & 0xff0000000000) >> 40;
        m_id[11] = (fifth & 0x00ff00000000) >> 32;
        m_id[12] = (fifth & 0x0000ff000000) >> 24;
        m_id[13] = (fifth & 0x000000ff0000) >> 16;
        m_id[14] = (fifth & 0x00000000ff00) >> 8;
        m_id[15] = fifth & 0x0000000000ff;
    }
   
    CS_ALWAYS_INLINE constexpr [[nodiscard]] bool operator<(const Uuid& rhs) const noexcept
    {
        if constexpr (helpers::isLittleEndianPlatform())
            return m_low < rhs.m_low || m_low == rhs.m_low && m_high < rhs.m_high;
        else
            return m_high < rhs.m_high || m_high == rhs.m_high && m_low < rhs.m_low;
    }

    CS_ALWAYS_INLINE constexpr [[nodiscard]] bool operator==(const Uuid& rhs) const noexcept
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
        std::size_t result = 0;
        for (size_t i = 0; i < sizeof(id.m_id); ++i)
            result ^= hashFunc(id.m_id[i]);
        
        return result;
    }
};
