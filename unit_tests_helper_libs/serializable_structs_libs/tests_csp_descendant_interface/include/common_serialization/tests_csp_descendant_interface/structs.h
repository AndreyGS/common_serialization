/**
 * @file common_serialization/tests_csp_descendant_interface/structs.h
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

#include <common_serialization/tests_csp_descendant_interface/interface.h>
#include <common_serialization/tests_csp_interface/structs.h>

namespace tests_csp_descendant_interface
{

template<typename T = void>
class SimpleStruct : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimpleStruct<>, T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimpleStruct<>, T>;

    static constexpr ags_cs::csp::Id kId{ 0xa4073aa8, 0xd9eb, 0x49cf, 0xb9be, 0xfea84ba9f314 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 1 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_i = 218418478;
    }

    uint32_t m_i{ 0 };

    [[nodiscard]] auto operator<=>(const SimpleStruct&) const = default;

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename T = void>
class DiamondDescendant : public tests_csp_interface::Diamond<ags_cs::GetCrtpMainType<DiamondDescendant<>, T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<DiamondDescendant<>, T>;

    static constexpr ags_cs::csp::Id kId{ 0x59a2dc70, 0x63eb, 0x434b, 0xbfe7, 0xad17dfad8e57 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 1 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    SimpleStruct<> m_sSt;

    [[nodiscard]] operator tests_csp_interface::Diamond<>& () noexcept
    {
        return  *static_cast<tests_csp_interface::Diamond<>*>(
            static_cast<void*>(
                static_cast<tests_csp_interface::Diamond<instance_type>*>(this)));
    }

    [[nodiscard]] operator const tests_csp_interface::Diamond<>& () const noexcept
    {
        return  *static_cast<const tests_csp_interface::Diamond<>*>(
            static_cast<const void*>(
                static_cast<const tests_csp_interface::Diamond<instance_type>*>(this)));
    }

    void fill()
    {
        tests_csp_interface::Diamond<instance_type>::fill();
        m_sSt.fill();
        m_ldouble = 0.982345792387523523l;
    }

    [[nodiscard]] auto operator<=>(const DiamondDescendant&) const = default;

    long double m_ldouble{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
};

} // namespace tests_csp_descendant_interface
