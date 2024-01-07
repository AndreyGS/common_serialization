/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/include/interface_for_test/SpecialTypesSerializableLegacy.h
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

namespace interface_for_test
{

namespace cs = common_serialization;

template<typename T = cs::Dummy>
class SimpleAssignableAlignedToOne_Version0 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleAssignableAlignedToOne_Version0<T>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleAssignableAlignedToOne_Version0<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId = SimpleAssignableAlignedToOne<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };

    SimpleAssignableAlignedToOne_Version0() { }
    template<typename T2>
    cs::Status init(const SimpleAssignableAlignedToOne_Version1<T2>& rhs);

    SimpleAssignableAlignedToOne_Version0& operator=(const SimpleAssignableAlignedToOne_Version0& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        m_ti.x = rhs.m_ti.x, m_ti.y = rhs.m_ti.y;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOne_Version0& rhs) const noexcept
    {
        return m_ti == rhs.m_ti;
    }

    not_part_of_interfaces::TwoInts m_ti{ 0 };

    friend cs::csp::processing::DataProcessor;
    friend SimpleAssignableAlignedToOne_Version1;
};

template<typename T = cs::Dummy>
class SimpleAssignable_Version0 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleAssignable_Version0<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleAssignable_Version0<>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId = SimpleAssignable<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;         // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };

    template<typename T2>
    cs::Status init(const SimpleAssignable<T2>& rhs);

    SimpleAssignable_Version0& operator=(const SimpleAssignable_Version0<>& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        m_i = rhs.m_i;
        m_j = rhs.m_j;
        m_saaToS = rhs.m_saaToS;
        m_saaToNS = rhs.m_saaToNS;
        m_saNS = rhs.m_saNS;
        memcpy(m_arrI32, rhs.m_arrI32, sizeof(m_arrI32));
        for (size_t i = 0; i < 3; ++i)
        {
            m_arrSaaTos[i].m_ti.x = rhs.m_arrSaaTos[i].m_ti.x;
            m_arrSaaTos[i].m_ti.y = rhs.m_arrSaaTos[i].m_ti.y;
        }
        for (size_t i = 0; i < 3; ++i)
        {
            m_arrSaaToNS[i].a = rhs.m_arrSaaToNS[i].a;
            m_arrSaaToNS[i].s = rhs.m_arrSaaToNS[i].s;
        }
        for (size_t i = 0; i < 3; ++i)
        {
            m_arrSaNS[i].q = rhs.m_arrSaNS[i].q;
            m_arrSaNS[i].w = rhs.m_arrSaNS[i].w;
        }
        m_vt = rhs.m_vt;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimpleAssignable_Version0& rhs)                  const noexcept
    {
        return 
               m_i == rhs.m_i
            && m_j == rhs.m_j
            && m_saaToS == rhs.m_saaToS
            && m_saaToNS == rhs.m_saaToNS
            && m_saNS == rhs.m_saNS

            && memcmp(m_arrI32, rhs.m_arrI32, sizeof(m_arrI32)) == 0

            && m_arrSaaTos[0] == rhs.m_arrSaaTos[0]
            && m_arrSaaTos[1] == rhs.m_arrSaaTos[1]
            && m_arrSaaTos[2] == rhs.m_arrSaaTos[2]

            && m_arrSaaToNS[0] == rhs.m_arrSaaToNS[0]
            && m_arrSaaToNS[1] == rhs.m_arrSaaToNS[1]
            && m_arrSaaToNS[2] == rhs.m_arrSaaToNS[2]

            && m_arrSaNS[0] == rhs.m_arrSaNS[0]
            && m_arrSaNS[1] == rhs.m_arrSaNS[1]
            && m_arrSaNS[2] == rhs.m_arrSaNS[2]

            && m_arrSaNS[0] == rhs.m_arrSaNS[0]
            && m_arrSaNS[1] == rhs.m_arrSaNS[1]
            && m_arrSaNS[2] == rhs.m_arrSaNS[2]
            
            && m_vt == rhs.m_vt
            ;
    }

    uint8_t m_i{ 0 };
    uint16_t m_j{ 0 };
    EmptyType<> m_et;
    not_part_of_interfaces::EmptyType m_et2;
    SimpleAssignableAlignedToOne_Version0<> m_saaToS;
    not_part_of_interfaces::SimpleAssignableAlignedToOne m_saaToNS;
    not_part_of_interfaces::SimpleAssignable m_saNS;

    uint32_t m_arrI32[3]{ 0 };
    EmptyType<> m_arrEtS[3]{};
    not_part_of_interfaces::EmptyType m_arrEtNS[3]{};
    SimpleAssignableAlignedToOne_Version0<> m_arrSaaTos[3]{};
    not_part_of_interfaces::SimpleAssignableAlignedToOne m_arrSaaToNS[3]{};
    not_part_of_interfaces::SimpleAssignable m_arrSaNS[3]{};

    uint32_t m_vt{ 0 };

    friend cs::csp::processing::DataProcessor;
    friend SimpleAssignable;
};

template<typename T1>
template<typename T2>
cs::Status SimpleAssignable_Version0<T1>::init(const SimpleAssignable<T2>& rhs)
{
    m_i = rhs.m_i;
    m_j = rhs.m_j;
    m_saaToS.m_ti.x = rhs.m_saaToS.m_x;
    m_saaToS.m_ti.y = rhs.m_saaToS.m_y;
    m_saaToNS = rhs.m_saaToNS;
    m_saNS = rhs.m_saNS;
    memcpy(m_arrI32, rhs.m_arrI32, sizeof(m_arrI32));
    for (size_t i = 0; i < 3; ++i)
    {
        m_arrSaaTos[i].m_ti.x = rhs.m_arrSaaTos[i].m_x;
        m_arrSaaTos[i].m_ti.y = rhs.m_arrSaaTos[i].m_y;
    }
    memcpy(m_arrSaaToNS, rhs.m_arrSaaToNS, sizeof(m_arrSaaToNS));
    memcpy(m_arrSaNS, rhs.m_arrSaNS, sizeof(m_arrSaNS));
    
    m_vt = rhs.m_vx/2;

    return cs::Status::kNoError;
}

template<typename T1>
template<typename T2>
cs::Status SimpleAssignable<T1>::init(const SimpleAssignable_Version0<T2>& rhs)
{
    m_i = rhs.m_i;
    m_j = rhs.m_j;
    m_saaToS.m_x = rhs.m_saaToS.m_ti.x;
    m_saaToS.m_y = rhs.m_saaToS.m_ti.y;
    m_saaToNS = rhs.m_saaToNS;
    m_saNS = rhs.m_saNS;
    memcpy(m_arrI32, rhs.m_arrI32, sizeof(m_arrI32));
    for (size_t i = 0; i < 3; ++i)
    {
        m_arrSaaTos[i].m_x = rhs.m_arrSaaTos[i].m_ti.x;
        m_arrSaaTos[i].m_y = rhs.m_arrSaaTos[i].m_ti.y;
    }

    memcpy(m_arrSaaToNS, rhs.m_arrSaaToNS, sizeof(m_arrSaaToNS));
    memcpy(m_arrSaNS, rhs.m_arrSaNS, sizeof(m_arrSaNS));

    m_vx = rhs.m_vt * 2;

    return cs::Status::kNoError;
}

template<typename T = cs::Dummy>
struct SimpleAssignableDescendant_Version0 : public SimpleAssignable_Version0<cs::GetCrtpMainType<SimpleAssignableDescendant_Version0<>, T>>
{
    using instance_type = cs::GetCrtpMainType<SimpleAssignableDescendant_Version0<>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId = SimpleAssignableDescendant<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };

    uint32_t m_d{ 0 };

    SimpleAssignableDescendant_Version0() { }
    template<typename T2>
    cs::Status init(const SimpleAssignableDescendant<T2>& rhs);

    [[nodiscard]] operator SimpleAssignable_Version0<>& () noexcept
    {
        return  *static_cast<SimpleAssignable_Version0<>*>(
            static_cast<void*>(
                static_cast<SimpleAssignable_Version0<instance_type>*>(this)));
    }

    [[nodiscard]] operator const SimpleAssignable_Version0<>& () const noexcept
    {
        return  *static_cast<const SimpleAssignable_Version0<>*>(
            static_cast<const void*>(
                static_cast<const SimpleAssignable_Version0<instance_type>*>(this)));
    }

    SimpleAssignableDescendant_Version0& operator=(const SimpleAssignableDescendant_Version0<>& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        SimpleAssignable_Version0<>::operator=(rhs);
        m_d = rhs.m_d;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimpleAssignableDescendant_Version0& rhs) const noexcept
    {
        return m_d == rhs.m_d && SimpleAssignable_Version0<instance_type>::operator==(rhs);
    }

    friend cs::csp::processing::DataProcessor;
    friend SimpleAssignableDescendant;
};


template<typename T1>
template<typename T2>
cs::Status SimpleAssignableDescendant_Version0<T1>::init(const SimpleAssignableDescendant<T2>& rhs)
{
    static_cast<SimpleAssignable_Version0<T1>&>(*this).init(rhs);
    m_d = rhs.m_d;

    return cs::Status::kNoError;
}

template<typename T1>
template<typename T2>
cs::Status SimpleAssignableDescendant<T1>::init(const SimpleAssignableDescendant_Version0<T2>& rhs)
{
    static_cast<SimpleAssignable<T2>&>(*this).init(rhs);
    m_d = rhs.m_d;

    return cs::Status::kNoError;
}

template<typename>
class SForAllModesTests_Version2;

template<typename T = cs::Dummy>
class SForAllModesTests_Version0 : public cs::csp::ISerializable<cs::GetCrtpMainType<SForAllModesTests_Version0<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SForAllModesTests_Version0<T>, T>;

    static constexpr cs::csp::Id kId = DForAllModesTests<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };

    template<typename T2>
    cs::Status init(const SForAllModesTests_Version2<T2>& rhs);

    [[nodiscard]] bool operator==(const SForAllModesTests_Version0<>& rhs) const noexcept
    {
        return
            m_saDs == rhs.m_saDs
            && m_diamond == rhs.m_diamond
            && m_sptCs == rhs.m_sptCs
            && m_saaToStS == rhs.m_saaToStS
            && m_saStS == rhs.m_saStS
            && m_stS == rhs.m_stS
            && m_mpt == rhs.m_mpt;

    }

    SimpleAssignableDescendant_Version0<> m_saDs;
    Diamond<> m_diamond;
    SpecialProcessingType<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType1<> m_saaToStS;
    SimpleAssignableSimilarType1<> m_saStS;
    SimilarType1<> m_stS;
    ManyPointersType<> m_mpt;

    friend cs::csp::processing::DataProcessor;
    friend SForAllModesTests_Version2;
};


template<typename T = cs::Dummy>
class SimpleAssignableAlignedToOne_Version1 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleAssignableAlignedToOne_Version1<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleAssignableAlignedToOne_Version1<>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId = SimpleAssignableAlignedToOne<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 1, 0 };

    SimpleAssignableAlignedToOne_Version1() { }
    template<typename T2>
    cs::Status init(const SimpleAssignableAlignedToOne_Version0<T2>& rhs)
    {
        m_x = rhs.m_ti.x;
        m_y = rhs.m_ti.y;

        return cs::Status::kNoError;
    }

    template<typename T2>
    cs::Status init(const SimpleAssignableAlignedToOne<T2>& rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;

        return cs::Status::kNoError;
    }

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOne_Version1& rhs) const noexcept
    {
        return m_x == rhs.m_x && m_y == rhs.m_y;
    }

    uint8_t m_x{ 0 };
    uint16_t m_y{ 0 };

    friend cs::csp::processing::DataProcessor;
    friend SimpleAssignableAlignedToOne_Version0;
    friend SimpleAssignableAlignedToOne;
};

template<typename T1>
template<typename T2>
cs::Status SimpleAssignableAlignedToOne_Version0<T1>::init(const SimpleAssignableAlignedToOne_Version1<T2>& rhs)
{
    m_ti.x = rhs.m_x;
    m_ti.y = rhs.m_y;

    return cs::Status::kNoError;
}

template<typename T1>
template<typename T2>
cs::Status SimpleAssignableAlignedToOne<T1>::init(const SimpleAssignableAlignedToOne_Version1<T2>& rhs)
{
    m_x = rhs.m_x;
    m_y = rhs.m_y;

    return cs::Status::kNoError;
}

template<typename T = cs::Dummy>
class SForAllModesTests_Version2 : public cs::csp::ISerializable<cs::GetCrtpMainType<SForAllModesTests_Version2<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SForAllModesTests_Version2<>, T>;

    static constexpr cs::csp::Id kId = DForAllModesTests<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 2;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 2, 0 };

    template<typename T2>
    cs::Status init(const SForAllModesTests_Version0<T2>& rhs);
    template<typename T2>
    cs::Status init(const DForAllModesTests<T2>& rhs);

    [[nodiscard]] bool operator==(const SForAllModesTests_Version2<>& rhs) const noexcept
    {
        return
               m_saS == rhs.m_saS
            && m_i == rhs.m_i
            && m_diamond == rhs.m_diamond
            && m_sptCs == rhs.m_sptCs
            && m_saaToStS == rhs.m_saaToStS
            && m_saStS == rhs.m_saStS
            && m_stS == rhs.m_stS
            && m_mpt == rhs.m_mpt;
    }

    Diamond<> m_diamond;
    SpecialProcessingType<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType1<> m_saaToStS;
    SimpleAssignableSimilarType1<> m_saStS;
    SimilarType1<> m_stS;
    ManyPointersType<> m_mpt;

    SimpleAssignable<> m_saS;
    int m_i{ 0 }; // duplicated m_saDs.m_i

    friend cs::csp::processing::DataProcessor;
    friend SForAllModesTests_Version0;
    friend DForAllModesTests;
};

template<typename T1>
template<typename T2>
cs::Status SForAllModesTests_Version0<T1>::init(const SForAllModesTests_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    static_cast<SimpleAssignable_Version0<>&>(m_saDs).init(rhs.m_saS);
    m_saDs.m_d = rhs.m_i;
    m_saDs.m_i += 1; // additional conversion

    return cs::Status::kNoError;
}

template<typename T1>
template<typename T2>
cs::Status SForAllModesTests_Version2<T1>::init(const SForAllModesTests_Version0<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saS.init(static_cast<const SimpleAssignable_Version0<>&>(rhs.m_saDs));
    m_i = rhs.m_saDs.m_d;
    m_saS.m_i -= 1; // additional conversion

    return cs::Status::kNoError;
}

template<typename T1>
template<typename T2>
cs::Status SForAllModesTests_Version2<T1>::init(const DForAllModesTests<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saS = rhs.m_saDs;
    m_saS.m_i += 1; // additional conversion
    m_i = rhs.m_saDs.m_d;

    return cs::Status::kNoError;
}

template<typename T1>
template<typename T2>
cs::Status DForAllModesTests<T1>::init(const SForAllModesTests_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    static_cast<SimpleAssignable<>&>(m_saDs) = rhs.m_saS;
    m_saDs.m_i -= 1; // additional conversion
    m_saDs.m_d = rhs.m_i;

    return cs::Status::kNoError;
}

} // namespace interface_for_test
