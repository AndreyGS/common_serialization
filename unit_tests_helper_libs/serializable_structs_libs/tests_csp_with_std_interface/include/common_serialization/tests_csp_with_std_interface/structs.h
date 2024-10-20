/**
 * @file common_serialization/tests_csp_with_std_interface/structs.h
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
#include <vector>
#include <map>
#include <tuple>
#include <common_serialization/csp_base/ISerializable.h>
#include <common_serialization/tests_csp_with_std_interface/interface.h>

namespace tests_csp_with_std_interface
{

void fill(std::string& output);
void fill(std::wstring& output);

template<typename T = void>
class OneBigType : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<OneBigType<>, T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<OneBigType<>, T>;

    static constexpr ags_cs::csp::Id kId{ 0x93f1547d, 0xeebd, 0x4281, 0x9f64, 0xd23008e2c0b3 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        tests_csp_with_std_interface::fill(m_string1);
        tests_csp_with_std_interface::fill(m_string2);
        m_vector1.push_back(m_string1 + "123");
        m_vector1.push_back(m_string1 + "456");
        m_vector1.push_back(m_string1 + "789");
        m_vector2.push_back(5);
        m_vector2.push_back(12);
        m_vector2.push_back(35456453);
        m_map1.emplace(std::make_pair(123, .324f));
        m_map1.emplace(std::make_pair(17, 65468.324f));
        m_map2.emplace(std::make_pair("wete", std::vector<uint8_t>{54, 124, 21, 19}));
        m_map2.emplace(std::make_pair("wtassac", std::vector<uint8_t>{23, 2, 0, 139}));
        m_map2.emplace(std::make_pair(" b34b", std::vector<uint8_t>{2, 78, 235, 16}));
        m_tuple1 = { 93, 3209857239, "099234" };
        m_tuple2 = { 35232632.2 };
    }

    [[nodiscard]] auto operator<=>(const OneBigType&) const = default;

    std::string m_string1;
    std::wstring m_string2;
    std::vector<std::string> m_vector1;
    std::vector<uint32_t> m_vector2;
    std::map<uint8_t, float> m_map1;
    std::map<std::string, std::vector<uint8_t>> m_map2;
    std::tuple<uint8_t, int64_t, std::string> m_tuple1;
    std::tuple<double> m_tuple2;
};

} // namespace tests_csp_with_std_interface
