/**
 * @file UnitTests/serializable_structs/restricted_structs/include/restricted_structs/Structs.h
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

namespace restricted_structs
{

namespace cs = common_serialization;

struct EmptyType
{
    using empty_type_tag = std::true_type;

    [[nodiscard]] auto operator<=>(const EmptyType&) const = default;
};

#pragma pack(push, 1)

struct SimplyAssignableAlignedToOne
{
    using simply_assignable_aligned_to_one_tag = std::true_type;

    [[nodiscard]] auto operator<=>(const SimplyAssignableAlignedToOne&) const = default;

    uint16_t a{ 0 };
    uint8_t s{ 0 };
};

#pragma pack(pop)

struct SimplyAssignable
{
    using simply_assignable_tag = std::true_type;

    [[nodiscard]] auto operator<=>(const SimplyAssignable&) const = default;

    uint16_t q{ 0 };
    uint64_t w{ 0 };
};

class DynamicPolymorphic
{
public:
    virtual ~DynamicPolymorphic() {}

    static constexpr cs::csp::Id kId{ 0xad460dc2, 0x429c, 0x4313, 0xa7c5, 0x06fca38389a0 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };

    [[nodiscard]] auto operator<=>(const DynamicPolymorphic&) const = default;

    uint8_t m_r{ 0 };
    uint8_t m_arrR[3]{ 0 };

    friend cs::csp::processing::data::BodyProcessor;
};


struct DiamondBase
{
    [[nodiscard]] auto operator<=>(const DiamondBase&) const = default;

    uint32_t m_d0{ 0 };
};

struct DiamondEdge1 : virtual public DiamondBase
{
    [[nodiscard]] auto operator<=>(const DiamondEdge1&) const = default;

    uint32_t m_d1{ 0 };
};

struct DiamondEdge2 : virtual public DiamondBase
{
    [[nodiscard]] auto operator<=>(const DiamondEdge2&) const = default;

    uint32_t m_d2{ 0 };
};

struct TwoInts
{
    using simply_assignable_tag = std::true_type;

    [[nodiscard]] auto operator<=>(const TwoInts&) const = default;

    uint8_t x{ 0 };
    uint16_t y{ 0 };
};

struct SimplyAssignableWithoutSerializationFunctions
{
    using simply_assignable_tag = std::true_type;

    [[nodiscard]] auto operator<=>(const SimplyAssignableWithoutSerializationFunctions&) const = default;

    uint8_t i{ 0 };
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

} // namespace restricted_structs