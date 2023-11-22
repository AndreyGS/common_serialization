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

struct TwoInts
{
    uint8_t x{ 0 };
    uint16_t y{ 0 };

    [[nodiscard]] bool operator == (const TwoInts& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y;
    }

    using simple_assignable_tag = std::true_type;
};

template<typename T = Dummy>
class SimpleAssignableAlignedToOneSerializable_Version0 : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version0<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version0<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr csp::name_hash_t kNameHash = 11;
    static constexpr csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    SimpleAssignableAlignedToOneSerializable_Version0() { }
    template<typename T2>
    Status init(const SimpleAssignableAlignedToOneSerializable_Version1<T2>& rhs);

    SimpleAssignableAlignedToOneSerializable_Version0& operator=(const SimpleAssignableAlignedToOneSerializable_Version0& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        m_ti.x = rhs.m_ti.x, m_ti.y = rhs.m_ti.y;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSerializable_Version0& rhs) const noexcept
    {
        return m_ti == rhs.m_ti;
    }

    TwoInts m_ti{ 0 };

    friend csp::processing::DataProcessor;
    friend SimpleAssignableAlignedToOneSerializable_Version1;
};

template<typename>
struct SimpleAssignableDescendantSerializable_Version1;

template<typename T = Dummy>
class SimpleAssignableSerializable_Version0 : public csp::ISerializable<GetCrtpMainType<SimpleAssignableSerializable_Version0<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableSerializable_Version0<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr csp::name_hash_t kNameHash = 2;
    static constexpr csp::interface_version_t kInterfaceVersion = 0;         // latest version among all dependable structs
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    template<typename T2>
    Status init(const SimpleAssignableSerializable<T2>& rhs);

    SimpleAssignableSerializable_Version0& operator=(const SimpleAssignableSerializable_Version0<>& rhs) noexcept
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

    [[nodiscard]] bool operator==(const SimpleAssignableSerializable_Version0& rhs)                  const noexcept
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
    EmptyTypeSerializable<> m_et;
    EmptyTypeNotSerializable m_et2;
    SimpleAssignableAlignedToOneSerializable_Version0<> m_saaToS;
    SimpleAssignableAlignedToOneNotSerializable m_saaToNS;
    SimpleAssignableNotSerializable m_saNS;

    uint32_t m_arrI32[3]{ 0 };
    EmptyTypeSerializable<> m_arrEtS[3]{};
    EmptyTypeNotSerializable m_arrEtNS[3]{};
    SimpleAssignableAlignedToOneSerializable_Version0<> m_arrSaaTos[3]{};
    SimpleAssignableAlignedToOneNotSerializable m_arrSaaToNS[3]{};
    SimpleAssignableNotSerializable m_arrSaNS[3]{};

    uint32_t m_vt{ 0 };

    friend csp::processing::DataProcessor;
    friend SimpleAssignableSerializable;
};

template<typename T1>
template<typename T2>
Status SimpleAssignableSerializable_Version0<T1>::init(const SimpleAssignableSerializable<T2>& rhs)
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

    return Status::kNoError;
}

template<typename T1>
template<typename T2>
Status SimpleAssignableSerializable<T1>::init(const SimpleAssignableSerializable_Version0<T2>& rhs)
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

    return Status::kNoError;
}

template<typename T = Dummy>
struct SimpleAssignableDescendantSerializable_Version0 : public SimpleAssignableSerializable_Version0<GetCrtpMainType<SimpleAssignableDescendantSerializable_Version0<T>, T>>
{
    using instance_type = GetCrtpMainType<SimpleAssignableDescendantSerializable_Version0<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr csp::name_hash_t kNameHash = 3;
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    uint32_t m_d{ 0 };

    SimpleAssignableDescendantSerializable_Version0() { }
    template<typename T2>
    Status init(const SimpleAssignableDescendantSerializable<T2>& rhs);

    [[nodiscard]] operator SimpleAssignableSerializable_Version0<>& () noexcept
    {
        return  *static_cast<SimpleAssignableSerializable_Version0<>*>(
            static_cast<void*>(
                static_cast<SimpleAssignableSerializable_Version0<instance_type>*>(this)));
    }

    [[nodiscard]] operator const SimpleAssignableSerializable_Version0<>& () const noexcept
    {
        return  *static_cast<const SimpleAssignableSerializable_Version0<>*>(
            static_cast<const void*>(
                static_cast<const SimpleAssignableSerializable_Version0<instance_type>*>(this)));
    }

    SimpleAssignableDescendantSerializable_Version0& operator=(const SimpleAssignableDescendantSerializable_Version0<>& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        SimpleAssignableSerializable_Version0<>::operator=(rhs);
        m_d = rhs.m_d;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimpleAssignableDescendantSerializable_Version0& rhs) const noexcept
    {
        return m_d == rhs.m_d && SimpleAssignableSerializable_Version0<instance_type>::operator==(rhs);
    }

    friend csp::processing::DataProcessor;
    friend SimpleAssignableDescendantSerializable;
};


template<typename T1>
template<typename T2>
Status SimpleAssignableDescendantSerializable_Version0<T1>::init(const SimpleAssignableDescendantSerializable<T2>& rhs)
{
    static_cast<SimpleAssignableSerializable_Version0<T1>&>(*this).init(rhs);
    m_d = rhs.m_d;

    return Status::kNoError;
}

template<typename T1>
template<typename T2>
Status SimpleAssignableDescendantSerializable<T1>::init(const SimpleAssignableDescendantSerializable_Version0<T2>& rhs)
{
    static_cast<SimpleAssignableSerializable<T2>&>(*this).init(rhs);
    m_d = rhs.m_d;

    return Status::kNoError;
}

template<typename>
class SForAllModesTests_Version2;

template<typename T = Dummy>
class SForAllModesTests_Version0 : public csp::ISerializable<GetCrtpMainType<SForAllModesTests_Version0<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SForAllModesTests_Version0<T>, T>;

    static constexpr csp::name_hash_t kNameHash = 10000;
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    template<typename T2>
    Status init(const SForAllModesTests_Version2<T2>& rhs);

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

    SimpleAssignableDescendantSerializable_Version0<> m_saDs;
    DiamondSerializable<> m_diamond;
    SpecialProcessingTypeContainSerializable<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType1Serializable<> m_saaToStS;
    SimpleAssignableSimilarType1Serializable<> m_saStS;
    SimilarType1Serializable<> m_stS;
    ManyPointersTypeSerializable<> m_mpt;

    friend csp::processing::DataProcessor;
    friend SForAllModesTests_Version2;
};


template<typename T = Dummy>
class SimpleAssignableAlignedToOneSerializable_Version1 : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version1<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version1<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr csp::name_hash_t kNameHash = 10;
    static constexpr csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 1, 0 };

    SimpleAssignableAlignedToOneSerializable_Version1() { }
    template<typename T2>
    Status init(const SimpleAssignableAlignedToOneSerializable_Version0<T2>& rhs)
    {
        m_x = rhs.m_ti.x;
        m_y = rhs.m_ti.y;

        return Status::kNoError;
    }

    template<typename T2>
    Status init(const SimpleAssignableAlignedToOneSerializable<T2>& rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;

        return Status::kNoError;
    }

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSerializable_Version1& rhs) const noexcept
    {
        return m_x == rhs.m_x && m_y == rhs.m_y;
    }

    uint8_t m_x{ 0 };
    uint16_t m_y{ 0 };

    friend csp::processing::DataProcessor;
    friend SimpleAssignableAlignedToOneSerializable_Version0;
    friend SimpleAssignableAlignedToOneSerializable;
};

template<typename T1>
template<typename T2>
Status SimpleAssignableAlignedToOneSerializable_Version0<T1>::init(const SimpleAssignableAlignedToOneSerializable_Version1<T2>& rhs)
{
    m_ti.x = rhs.m_x;
    m_ti.y = rhs.m_y;

    return Status::kNoError;
}

template<typename T1>
template<typename T2>
Status SimpleAssignableAlignedToOneSerializable<T1>::init(const SimpleAssignableAlignedToOneSerializable_Version1<T2>& rhs)
{
    m_x = rhs.m_x;
    m_y = rhs.m_y;

    return Status::kNoError;
}

template<typename T = Dummy>
class SForAllModesTests_Version2 : public csp::ISerializable<GetCrtpMainType<SForAllModesTests_Version2<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SForAllModesTests_Version2<T>, T>;

    static constexpr csp::name_hash_t kNameHash = 10000;
    static constexpr csp::interface_version_t kInterfaceVersion = 2;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 2, 0 };

    template<typename T2>
    Status init(const SForAllModesTests_Version0<T2>& rhs);
    template<typename T2>
    Status init(const DForAllModesTests<T2>& rhs);

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

    DiamondSerializable<> m_diamond;
    SpecialProcessingTypeContainSerializable<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType1Serializable<> m_saaToStS;
    SimpleAssignableSimilarType1Serializable<> m_saStS;
    SimilarType1Serializable<> m_stS;
    ManyPointersTypeSerializable<> m_mpt;

    SimpleAssignableSerializable<> m_saS;
    int m_i{ 0 }; // duplicated m_saDs.m_i

    friend csp::processing::DataProcessor;
    friend SForAllModesTests_Version0;
    friend DForAllModesTests;
};

template<typename T1>
template<typename T2>
Status SForAllModesTests_Version0<T1>::init(const SForAllModesTests_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    static_cast<SimpleAssignableSerializable_Version0<>&>(m_saDs).init(rhs.m_saS);
    m_saDs.m_d = rhs.m_i;
    m_saDs.m_i += 1; // additional conversion

    return Status::kNoError;
}

template<typename T1>
template<typename T2>
Status SForAllModesTests_Version2<T1>::init(const SForAllModesTests_Version0<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saS.init(static_cast<const SimpleAssignableSerializable_Version0<>&>(rhs.m_saDs));
    m_i = rhs.m_saDs.m_d;
    m_saS.m_i -= 1; // additional conversion

    return Status::kNoError;
}

template<typename T1>
template<typename T2>
Status SForAllModesTests_Version2<T1>::init(const DForAllModesTests<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS.m_j = rhs.m_saaToStS.m_j;
    m_saaToStS.m_k = rhs.m_saaToStS.m_k;
    m_saStS.m_j = rhs.m_saStS.m_j;
    m_saStS.m_k = rhs.m_saStS.m_k;
    m_stS.m_j = rhs.m_stS.m_j;
    m_stS.m_k = rhs.m_stS.m_k;
    m_mpt = rhs.m_mpt;

    m_saS = rhs.m_saDs;
    m_saS.m_i += 1; // additional conversion
    m_i = rhs.m_saDs.m_d;

    return Status::kNoError;
}

template<typename T1>
template<typename T2>
Status DForAllModesTests<T1>::init(const SForAllModesTests_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS.m_j = rhs.m_saaToStS.m_j;
    m_saaToStS.m_k = rhs.m_saaToStS.m_k;
    m_saStS.m_j = rhs.m_saStS.m_j;
    m_saStS.m_k = rhs.m_saStS.m_k;
    m_stS.m_j = rhs.m_stS.m_j;
    m_stS.m_k = rhs.m_stS.m_k;
    m_mpt = rhs.m_mpt;

    static_cast<SimpleAssignableSerializable<>&>(m_saDs) = rhs.m_saS;
    m_saDs.m_i -= 1; // additional conversion
    m_saDs.m_d = rhs.m_i;

    return Status::kNoError;
}

} // namespace special_types
