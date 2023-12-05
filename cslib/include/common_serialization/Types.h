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

struct Uuid
{
    uint64_t leftPart{ 0 };
    uint64_t rightPart{ 0 };
   
    constexpr bool operator<(const Uuid& rhs) const noexcept
    {
        return leftPart < rhs.leftPart || leftPart == rhs.leftPart && rightPart < rhs.rightPart;
    }

    constexpr bool operator==(const Uuid& rhs) const noexcept
    {
        return leftPart == rhs.leftPart && rightPart == rhs.rightPart;
    }
};


} // namespace common_serialization

template<>
struct std::hash<common_serialization::Uuid>
{
    std::size_t operator()(const common_serialization::Uuid& uuid) const noexcept
    {
        std::size_t hash1 = std::hash<uint64_t>{}(uuid.leftPart);
        std::size_t hash2 = std::hash<uint64_t>{}(uuid.rightPart);
        return hash1 ^ hash2;
    }
};
