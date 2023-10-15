/**
 * @file SpecialTypesSerializableLegacy.h
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

namespace special_types
{

using namespace common_serialization;

template<typename T = Dummy>
class SimpleAssignableAlignedToOneSerializable_Version1 : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version1<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version1<T>, T>;
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 10;
    static constexpr uint32_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr csp::traits::NameHashAndVersion kVersionsHierarchy[] = { { 10, 1 }, { 11, 0 } };

    [[nodiscard]] uint8_t& getX()                 noexcept { return m_x; }    // getters here are only need for testing proposes
    [[nodiscard]] const uint8_t& getX()     const noexcept { return m_x; }    // (not required for serialization itself)
    [[nodiscard]] uint8_t& getY()                 noexcept { return m_y; }
    [[nodiscard]] const uint8_t& getY()     const noexcept { return m_y; }

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSerializable_Version1& rhs) const noexcept
    {
        return m_x == rhs.m_x && m_y == rhs.m_y;
    }

private:
    uint8_t m_x{ 0 };
    uint16_t m_y{ 0 };

    friend csp::processing::DataProcessor;
};

struct TwoInts
{
    uint8_t x{ 0 };
    uint16_t y{ 0 };
};

template<typename T = Dummy>
class SimpleAssignableAlignedToOneSerializable_Version0 : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version0<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version0<T>, T>;
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 11;
    static constexpr uint32_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr csp::traits::NameHashAndVersion kVersionsHierarchy[] = { { 11, 0 } };

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSerializable_Version0& rhs) const noexcept
    {
        return m_ti == rhs.m_ti;
    }

private:
    TwoInts m_ti{ 0 };

    friend csp::processing::DataProcessor;
};

} // namespace special_types
