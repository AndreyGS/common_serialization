/**
 * @file UnitTests/SerializableStructs/DescendantInterface/include/descendant_interface/Structs.h
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

namespace descendant_interface
{

template<typename T = cs::Dummy>
class SimpleStruct : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleStruct<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleStruct<>, T>;

    static constexpr cs::csp::Id kId{ 0xa4073aa8, 0xd9eb, 0x49cf, 0xb9be, 0xfea84ba9f314 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 1 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

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

    friend cs::csp::processing::data::BodyProcessor;
};

template<typename T = cs::Dummy>
class DiamondDescendant : public interface_for_test::Diamond<cs::GetCrtpMainType<DiamondDescendant<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<DiamondDescendant<>, T>;

    static constexpr cs::csp::Id kId{ 0x59a2dc70, 0x63eb, 0x434b, 0xbfe7, 0xad17dfad8e57 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 1 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    SimpleStruct<> m_sSt;

    [[nodiscard]] operator interface_for_test::Diamond<>& () noexcept
    {
        return  *static_cast<interface_for_test::Diamond<>*>(
            static_cast<void*>(
                static_cast<interface_for_test::Diamond<instance_type>*>(this)));
    }

    [[nodiscard]] operator const interface_for_test::Diamond<>& () const noexcept
    {
        return  *static_cast<const interface_for_test::Diamond<>*>(
            static_cast<const void*>(
                static_cast<const interface_for_test::Diamond<instance_type>*>(this)));
    }

    DiamondDescendant& operator=(const DiamondDescendant<>& rhs)
    {
        if (this == &rhs)
            return *this;

        m_sSt = rhs.m_sSt;

        return *this;
    }

    [[nodiscard]] bool operator==(const DiamondDescendant& rhs) const noexcept
    {
        return m_sSt == rhs.m_sSt;;
    }

    friend cs::csp::processing::data::BodyProcessor;
};

} // namespace descendant_interface
