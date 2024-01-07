/**
 * @file UnitTests/SerializableStructs/AnotherYetInterface/include/another_yet_interface/SpecialTypesSerializable.h
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

namespace another_yet_interface
{

namespace cs = common_serialization;

template<typename T = cs::Dummy>
class SimpleStruct : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleStruct<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleStruct<>, T>;

    static constexpr cs::csp::Id kId = cs::helpers::getUuid(0xfb2215a8, 0x9050, 0x4e5a, 0x8e1c, 0x7c836dba50bd);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };

    uint32_t m_i{ 0 };

    SimpleStruct& operator=(const SimpleStruct<>& rhs)
    {
        if (this == &rhs)
            return *this;

        m_i = rhs.m_i;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimpleStruct& rhs) const noexcept
    {
        return m_i == rhs.m_i;
    }

    friend cs::csp::processing::DataProcessor;
};

} // namespace another_yet_interface
