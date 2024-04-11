/**
 * @file UnitTests/SerializableStructs/WithStdIncludedInterface/include/with_std_included_interface/Structs.h
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

namespace with_std_included_interface
{

template<typename T = cs::Dummy>
class OneBigType : public cs::csp::ISerializable<cs::GetCrtpMainType<OneBigType<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<OneBigType<>, T>;

    static constexpr cs::csp::Id kId{ 0x93f1547d, 0xeebd, 0x4281, 0x9f64, 0xd23008e2c0b3 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    bool operator==(const OneBigType& rhs) const noexcept
    {
        if (m_string1 != rhs.m_string1 || m_string2 != rhs.m_string2 
            || m_vector1.size() != rhs.m_vector1.size() 
            || m_vector2.size() != rhs.m_vector2.size()
            || m_map1.size() != rhs.m_map1.size()
            || m_map2.size() != rhs.m_map2.size()
        )
            return false;

        for (size_t i = 0; i < m_vector1.size(); ++i)
            if (m_vector1[i] != rhs.m_vector1[i])
                return false;

        for (size_t i = 0; i < m_vector2.size(); ++i)
            if (m_vector2[i] != rhs.m_vector2[i])
                return false;

        try
        {
            for (auto [key, value] : m_map1)
                if (m_map1[key] != rhs.m_map1[key])
                    return false;

            for (auto [key, value] : m_map2)
                if (m_map2[key] != rhs.m_map2[key])
                    return false;
        }
        catch (...)
        {
            return false;
        }

        if (   std::get<0>(m_tuple1) != std::get<0>(rhs.m_tuple1)
            || std::get<1>(m_tuple1) != std::get<1>(rhs.m_tuple1)
            || std::get<2>(m_tuple1) != std::get<2>(rhs.m_tuple1)
            || std::get<0>(m_tuple2) != std::get<0>(rhs.m_tuple2)
        )
            return false;

        return true;
    }

    std::string m_string1;
    std::wstring m_string2;
    std::vector<std::string> m_vector1;
    std::vector<uint32_t> m_vector2;
    std::map<uint8_t, float> m_map1;
    std::map<std::string, std::vector<uint8_t>> m_map2;
    std::tuple<uint8_t, int64_t, std::string> m_tuple1;
    std::tuple<double> m_tuple2;
};

} // namespace with_std_included_interface
