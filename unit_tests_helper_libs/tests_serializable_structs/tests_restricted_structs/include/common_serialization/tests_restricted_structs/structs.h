/**
 * @file UnitTests/serializable_structs/tests_restricted_structs/include/tests_restricted_structs/structs.h
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

#include <string>

namespace tests_restricted_structs
{

namespace ags_cs = common_serialization;

template<typename T>
void fill(T& input);
template<>
void fill(std::string& output);
template<>
void fill(std::wstring& output);

struct EmptyType
{
    using empty_type_tag = std::true_type;

    [[nodiscard]] auto operator<=>(const EmptyType&) const = default;
};

#pragma pack(push, 1)

struct SimplyAssignableAlignedToOne
{
    using simply_assignable_aligned_to_one_tag = std::true_type;

    void fill();
    [[nodiscard]] auto operator<=>(const SimplyAssignableAlignedToOne&) const = default;

    uint16_t m_a{ 0 };
    uint8_t m_s{ 0 };
};

#pragma pack(pop)

struct SimplyAssignable
{
    using simply_assignable_tag = std::true_type;

    void fill();
    [[nodiscard]] auto operator<=>(const SimplyAssignable&) const = default;

    uint16_t m_q{ 0 };
    uint64_t m_w{ 0 };
};

struct DynamicPolymorphic
{
    virtual ~DynamicPolymorphic() {}

    void fill();
    [[nodiscard]] auto operator<=>(const DynamicPolymorphic&) const = default;

    uint8_t m_r{ 0 };
    uint8_t m_arrR[3]{ 0 };
};


struct DiamondBase
{
    void fill();
    [[nodiscard]] auto operator<=>(const DiamondBase&) const = default;

    uint32_t m_d0{ 0 };
};

struct DiamondEdge1 : virtual DiamondBase
{
    void fill();
    [[nodiscard]] auto operator<=>(const DiamondEdge1&) const = default;

    uint32_t m_d1{ 0 };
};

struct DiamondEdge2 : virtual DiamondBase
{
    void fill();
    [[nodiscard]] auto operator<=>(const DiamondEdge2&) const = default;

    uint32_t m_d2{ 0 };
};

struct TwoInts
{
    using simply_assignable_tag = std::true_type;

    void fill();
    [[nodiscard]] auto operator<=>(const TwoInts&) const = default;

    uint8_t m_x{ 0 };
    uint16_t m_y{ 0 };
};

struct SimplyAssignableWithoutSerializationFunctions
{
    using simply_assignable_tag = std::true_type;

    void fill();
    [[nodiscard]] auto operator<=>(const SimplyAssignableWithoutSerializationFunctions&) const = default;

    uint8_t m_i{ 0 };
};

struct RecursiveTestSpecial1;

struct RecursiveTestSpecial2
{
    int* pI{ nullptr };
    RecursiveTestSpecial2* pNext{ nullptr };
    RecursiveTestSpecial1* pAny{ nullptr };
};

struct RecursiveTestSpecial1
{
    RecursiveTestSpecial2* pAny{ nullptr };
};

} // namespace tests_restricted_structs
