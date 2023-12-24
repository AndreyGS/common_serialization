/**
 * @file UnitTests/SerializableStructs/NotPartOfInterfaces/include/not_part_of_interfaces/Structs.h
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

namespace not_part_of_interfaces
{

namespace cs = common_serialization;

struct EmptyType
{
    using empty_type_tag = std::true_type;
};

#pragma pack(push, 1)

struct SimpleAssignableAlignedToOne
{
    using simple_assignable_tag = std::true_type;

    uint16_t a{ 0 };
    uint8_t s{ 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOne& rhs) const noexcept
    {
        return a == rhs.a && s == rhs.s;
    }
};

#pragma pack(pop)

struct SimpleAssignable
{
    using simple_assignable_tag = std::true_type;

    uint16_t q{ 0 };
    uint64_t w{ 0 };

    [[nodiscard]] bool operator==(const SimpleAssignable& rhs) const noexcept
    {
        return q == rhs.q && w == rhs.w;
    }
};

class DynamicPolymorphic
{
public:
    virtual ~DynamicPolymorphic() {}

    static constexpr cs::csp::Id kId = cs::helpers::getUuid(0xad460dc2, 0x429c, 0x4313, 0xa7c5, 0x06fca38389a0);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };

    [[nodiscard]] bool operator==(const DynamicPolymorphic& rhs) const noexcept
    {
        return m_r == rhs.m_r && memcmp(m_arrR, rhs.m_arrR, sizeof(m_arrR)) == 0;
    }

    uint8_t m_r{ 0 };
    uint8_t m_arrR[3]{ 0 };

    friend cs::csp::processing::DataProcessor;
};


struct DiamondBase
{
    [[nodiscard]] bool operator==(const DiamondBase& rhs) const noexcept
    {
        return m_d0 == rhs.m_d0;
    }

    uint32_t m_d0{ 0 };
};

struct DiamondEdge1 : virtual public DiamondBase
{
    [[nodiscard]] bool operator==(const DiamondEdge1& rhs) const noexcept
    {
        return m_d1 == rhs.m_d1;
    }

    uint32_t m_d1{ 0 };

    friend cs::csp::processing::DataProcessor;
};

struct DiamondEdge2 : virtual public DiamondBase
{
    [[nodiscard]] bool operator==(const DiamondEdge2& rhs) const noexcept
    {
        return m_d2 == rhs.m_d2;
    }

    uint32_t m_d2{ 0 };

    friend cs::csp::processing::DataProcessor;
};

struct TwoInts
{
    using simple_assignable_tag = std::true_type;

    uint8_t x{ 0 };
    uint16_t y{ 0 };

    [[nodiscard]] bool operator == (const TwoInts& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y;
    }
};

} // namespace not_part_of_interface