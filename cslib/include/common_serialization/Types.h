/**
 * @file cslib/include/common_serialization/Types.h
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

namespace common_serialization
{

#pragma pack(push, 1)

/// @brief Struct for holding UUIDv4
struct Uuid
{
    using simple_assignable_tag = std::true_type;

    uint64_t leftPart{ 0 };
    uint64_t rightPart{ 0 };

    constexpr Uuid() {}
    constexpr Uuid(const Uuid& rhs)
        : leftPart(rhs.leftPart), rightPart(rhs.rightPart)
    { }

    constexpr Uuid(uint64_t lP, uint64_t rP) : leftPart(lP), rightPart(rP) { }
   
    constexpr bool operator<(const Uuid& rhs) const noexcept
    {
        return leftPart < rhs.leftPart || leftPart == rhs.leftPart && rightPart < rhs.rightPart;
    }

    constexpr bool operator==(const Uuid& rhs) const noexcept
    {
        return leftPart == rhs.leftPart && rightPart == rhs.rightPart;
    }
};

#pragma pack(pop)

constexpr Uuid kNullUuid{ 0, 0 };

} // namespace common_serialization

template<>
struct std::hash<common_serialization::Uuid>
{
    std::size_t operator()(const common_serialization::Uuid& id) const noexcept
    {
        std::size_t hash1 = std::hash<uint64_t>{}(id.leftPart);
        std::size_t hash2 = std::hash<uint64_t>{}(id.rightPart);
        return hash1 ^ hash2;
    }
};
