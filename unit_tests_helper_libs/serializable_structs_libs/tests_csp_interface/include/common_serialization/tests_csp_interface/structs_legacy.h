/**
 * @file common_serialization/tests_csp_interface/structs_legacy.h
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

#include <common_serialization/tests_csp_interface/structs.h>

namespace tests_csp_interface
{

namespace ags_cs = common_serialization;

template<typename T = ags_cs::Dummy>
class SimplyAssignableAlignedToOne_Version0 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOne_Version0<T>, T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOne_Version0<T>, T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId = SimplyAssignableAlignedToOne<>::getId();
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    ags_cs::Status init(const SimplyAssignableAlignedToOne_Version1<T2>& rhs);

    void fill()
    {
        m_ti.fill();
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableAlignedToOne_Version0&) const = default;

    tests_restricted_structs::TwoInts m_ti{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableAlignedToOne_Version1<T>;
};

template<typename T = ags_cs::Dummy>
class SimplyAssignable_Version0 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignable_Version0<>, T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignable_Version0<>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId = SimplyAssignable<>::getId();
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;         // latest version among all dependable structs
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    ags_cs::Status init(const SimplyAssignable<T2>& rhs);

    void fill()
    {
        m_i = 7;
        m_j = 8;

        m_saaToS.fill();
        m_saaToNS.fill();
        m_saNS.fill();

        memcpy(m_arrI32, "123456789012", sizeof(m_arrI32));
        m_arrSaaTos[0].m_ti.m_x = 48;
        m_arrSaaTos[0].m_ti.m_y = 60;
        m_arrSaaTos[1].m_ti.m_x = 49;
        m_arrSaaTos[1].m_ti.m_y = 61;
        m_arrSaaTos[2].m_ti.m_x = 50;
        m_arrSaaTos[2].m_ti.m_y = 62;

        m_arrSaaToNS[0].m_a = 15000;
        m_arrSaaToNS[0].m_s = 122;
        m_arrSaaToNS[1].m_a = 15001;
        m_arrSaaToNS[1].m_s = 123;
        m_arrSaaToNS[2].m_a = 15002;
        m_arrSaaToNS[2].m_s = 124;

        m_arrSaNS[0].m_q = 123;
        m_arrSaNS[0].m_w = 456;
        m_arrSaNS[1].m_q = 567;
        m_arrSaNS[1].m_w = 890;
        m_arrSaNS[2].m_q = 789;
        m_arrSaNS[2].m_w = 012;

        m_vt = 23492;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignable_Version0&) const = default;

    uint8_t m_i{ 0 };
    uint16_t m_j{ 0 };
    EmptyType<> m_et;
    tests_restricted_structs::EmptyType m_et2;
    SimplyAssignableAlignedToOne_Version0<> m_saaToS;
    tests_restricted_structs::SimplyAssignableAlignedToOne m_saaToNS;
    tests_restricted_structs::SimplyAssignable m_saNS;

    uint32_t m_arrI32[3]{ 0 };
    EmptyType<> m_arrEtS[3]{};
    tests_restricted_structs::EmptyType m_arrEtNS[3]{};
    SimplyAssignableAlignedToOne_Version0<> m_arrSaaTos[3]{};
    tests_restricted_structs::SimplyAssignableAlignedToOne m_arrSaaToNS[3]{};
    tests_restricted_structs::SimplyAssignable m_arrSaNS[3]{};

    uint32_t m_vt{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignable<T>;
};

template<typename T1>
template<typename T2>
ags_cs::Status SimplyAssignable_Version0<T1>::init(const SimplyAssignable<T2>& rhs)
{
    m_i = rhs.m_i;
    m_j = rhs.m_j;
    m_saaToS.m_ti.m_x = rhs.m_saaToS.m_x;
    m_saaToS.m_ti.m_y = rhs.m_saaToS.m_y;
    m_saaToNS = rhs.m_saaToNS;
    m_saNS = rhs.m_saNS;
    memcpy(m_arrI32, rhs.m_arrI32, sizeof(m_arrI32));
    for (size_t i = 0; i < 3; ++i)
    {
        m_arrSaaTos[i].m_ti.m_x = rhs.m_arrSaaTos[i].m_x;
        m_arrSaaTos[i].m_ti.m_y = rhs.m_arrSaaTos[i].m_y;
    }
    memcpy(m_arrSaaToNS, rhs.m_arrSaaToNS, sizeof(m_arrSaaToNS));
    memcpy(m_arrSaNS, rhs.m_arrSaNS, sizeof(m_arrSaNS));
    
    m_vt = rhs.m_vx/2;

    return ags_cs::Status::NoError;
}

template<typename T1>
template<typename T2>
ags_cs::Status SimplyAssignable<T1>::init(const SimplyAssignable_Version0<T2>& rhs)
{
    m_i = rhs.m_i;
    m_j = rhs.m_j;
    m_saaToS.m_x = rhs.m_saaToS.m_ti.m_x;
    m_saaToS.m_y = rhs.m_saaToS.m_ti.m_y;
    m_saaToNS = rhs.m_saaToNS;
    m_saNS = rhs.m_saNS;
    memcpy(m_arrI32, rhs.m_arrI32, sizeof(m_arrI32));
    for (size_t i = 0; i < 3; ++i)
    {
        m_arrSaaTos[i].m_x = rhs.m_arrSaaTos[i].m_ti.m_x;
        m_arrSaaTos[i].m_y = rhs.m_arrSaaTos[i].m_ti.m_y;
    }

    memcpy(m_arrSaaToNS, rhs.m_arrSaaToNS, sizeof(m_arrSaaToNS));
    memcpy(m_arrSaNS, rhs.m_arrSaNS, sizeof(m_arrSaNS));

    m_vx = rhs.m_vt * 2;

    return ags_cs::Status::NoError;
}

template<typename T = ags_cs::Dummy>
struct SimplyAssignableDescendant_Version0 : public SimplyAssignable_Version0<ags_cs::GetCrtpMainType<SimplyAssignableDescendant_Version0<>, T>>
{
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableDescendant_Version0<>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId = SimplyAssignableDescendant<>::getId();
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    uint32_t m_d{ 0 };

    template<typename T2>
    ags_cs::Status init(const SimplyAssignableDescendant<T2>& rhs);

    [[nodiscard]] operator SimplyAssignable_Version0<>& () noexcept
    {
        return  *static_cast<SimplyAssignable_Version0<>*>(
            static_cast<void*>(
                static_cast<SimplyAssignable_Version0<instance_type>*>(this)));
    }

    [[nodiscard]] operator const SimplyAssignable_Version0<>& () const noexcept
    {
        return  *static_cast<const SimplyAssignable_Version0<>*>(
            static_cast<const void*>(
                static_cast<const SimplyAssignable_Version0<instance_type>*>(this)));
    }

    void fill()
    {
        SimplyAssignable_Version0<instance_type>::fill();
        m_d = this->m_i + 2;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableDescendant_Version0&) const = default;

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableDescendant<T>;
};


template<typename T1>
template<typename T2>
ags_cs::Status SimplyAssignableDescendant_Version0<T1>::init(const SimplyAssignableDescendant<T2>& rhs)
{
    static_cast<SimplyAssignable_Version0<T1>&>(*this).init(rhs);
    m_d = rhs.m_d;

    return ags_cs::Status::NoError;
}

template<typename T1>
template<typename T2>
ags_cs::Status SimplyAssignableDescendant<T1>::init(const SimplyAssignableDescendant_Version0<T2>& rhs)
{
    static_cast<SimplyAssignable<T2>&>(*this).init(rhs);
    m_d = rhs.m_d;

    return ags_cs::Status::NoError;
}

#pragma pack(push, 1)

template<typename T = ags_cs::Dummy>
class AlwaysSimplyAssignable_Version0 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<AlwaysSimplyAssignable_Version0<T>, T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<AlwaysSimplyAssignable_Version0<T>, T>;
    using always_simply_assignable_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId = AlwaysSimplyAssignable<>::getId();
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    ags_cs::Status init(const AlwaysSimplyAssignable<T2>& rhs);

    void fill()
    {
        m_xx = 211;
        m_yy = 117;
    }

    [[nodiscard]] auto operator<=>(const AlwaysSimplyAssignable_Version0&) const = default;

    uint16_t m_xx{ 0 };
    uint16_t m_yy{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableAlignedToOne<T>;
};

#pragma pack(pop)

template<typename T1>
template<typename T2>
ags_cs::Status AlwaysSimplyAssignable_Version0<T1>::init(const AlwaysSimplyAssignable<T2>& rhs)
{
    m_xx = rhs.m_x + 1;
    m_yy = rhs.m_y + 2;

    return ags_cs::Status::NoError;
}

template<typename T1>
template<typename T2>
ags_cs::Status AlwaysSimplyAssignable<T1>::init(const AlwaysSimplyAssignable_Version0<T2>& rhs)
{
    m_x = rhs.m_xx - 1;
    m_y = rhs.m_yy - 2;

    return ags_cs::Status::NoError;
}

template<typename>
class SForAllModesTests_Version2;

template<typename T = ags_cs::Dummy>
class SForAllModesTests_Version0 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SForAllModesTests_Version0<T>, T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SForAllModesTests_Version0<T>, T>;

    static constexpr ags_cs::csp::Id kId = DForAllModesTests<>::getId();
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    ags_cs::Status init(const SForAllModesTests_Version2<T2>& rhs);

    void fill()
    {
        m_saDs.fill();

        m_diamond.fill();
        m_sptCs.fill();
        m_saaToStS.fill();
        m_saStS.fill();
        m_stS.fill();
        m_mpt.fill();
    }

    [[nodiscard]] auto operator<=>(const SForAllModesTests_Version0&) const = default;

    SimplyAssignableDescendant_Version0<> m_saDs;
    Diamond<> m_diamond;
    SpecialProcessingType<> m_sptCs;
    SimplyAssignableAlignedToOneSimilarType1<> m_saaToStS;
    SimplyAssignableSimilarType1<> m_saStS;
    SimilarType1<> m_stS;
    ManyPointersType<> m_mpt;

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SForAllModesTests_Version2<T>;
};

template<typename T = ags_cs::Dummy>
class SimplyAssignableAlignedToOne_Version1 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOne_Version1<>, T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOne_Version1<>, T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId = SimplyAssignableAlignedToOne<>::getId();
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 1, 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    ags_cs::Status init(const SimplyAssignableAlignedToOne_Version0<T2>& rhs)
    {
        m_x = rhs.m_ti.m_x;
        m_y = rhs.m_ti.m_y;

        return ags_cs::Status::NoError;
    }

    template<typename T2>
    ags_cs::Status init(const SimplyAssignableAlignedToOne<T2>& rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;

        return ags_cs::Status::NoError;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableAlignedToOne_Version1&) const = default;

    uint8_t m_x{ 0 };
    uint16_t m_y{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableAlignedToOne_Version0<T>;
    friend SimplyAssignableAlignedToOne<T>;
};

template<typename T1>
template<typename T2>
ags_cs::Status SimplyAssignableAlignedToOne_Version0<T1>::init(const SimplyAssignableAlignedToOne_Version1<T2>& rhs)
{
    m_ti.m_x = rhs.m_x;
    m_ti.m_y = rhs.m_y;

    return ags_cs::Status::NoError;
}

template<typename T1>
template<typename T2>
ags_cs::Status SimplyAssignableAlignedToOne<T1>::init(const SimplyAssignableAlignedToOne_Version1<T2>& rhs)
{
    m_x = rhs.m_x;
    m_y = rhs.m_y;

    return ags_cs::Status::NoError;
}

template<typename T = ags_cs::Dummy>
class SimplyAssignableFixedSize_Version1 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableFixedSize_Version1<T>, T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableFixedSize_Version1<T>, T>;
    using simply_assignable_fixed_size_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId = SimplyAssignableFixedSize<>::getId();
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 1 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    ags_cs::Status init(const SimplyAssignableFixedSize<T2>& rhs);

    void fill()
    {
        m_xx = 179;
        m_asa.fill();
        m_arrAsa[0].m_xx = 31;
        m_arrAsa[0].m_yy = 134;
        m_arrAsa[1].m_xx = 89;
        m_arrAsa[1].m_yy = 55;
        m_arrAsa[2].m_xx = 160;
        m_arrAsa[2].m_yy = 229;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableFixedSize_Version1&) const = default;

    uint16_t m_xx{ 0 };
    AlwaysSimplyAssignable_Version0<> m_asa;
    AlwaysSimplyAssignable_Version0<> m_arrAsa[3];

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableFixedSize<T>;
};

template<typename T1>
template<typename T2>
ags_cs::Status SimplyAssignableFixedSize_Version1<T1>::init(const SimplyAssignableFixedSize<T2>& rhs)
{
    m_xx = rhs.m_x + 1;
    m_asa.init(rhs.m_asa);

    for (size_t i = 0; i < 3; ++i)
        m_arrAsa[i].init(rhs.m_arrAsa[i]);

    return ags_cs::Status::NoError;
}

template<typename T1>
template<typename T2>
ags_cs::Status SimplyAssignableFixedSize<T1>::init(const SimplyAssignableFixedSize_Version1<T2>& rhs)
{
    m_x = rhs.m_xx - 1;
    m_asa.init(rhs.m_asa);

    for (size_t i = 0; i < 3; ++i)
        m_arrAsa[i].init(rhs.m_arrAsa[i]);

    return ags_cs::Status::NoError;
}

template<typename T = ags_cs::Dummy>
class SForAllModesTests_Version2 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SForAllModesTests_Version2<>, T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SForAllModesTests_Version2<>, T>;

    static constexpr ags_cs::csp::Id kId = DForAllModesTests<>::getId();
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 2;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 2, 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    ags_cs::Status init(const SForAllModesTests_Version0<T2>& rhs);
    template<typename T2>
    ags_cs::Status init(const DForAllModesTests<T2>& rhs);

    void fill()
    {
        m_saS.fill();
        m_i = m_saS.m_i + 2;      // == SimplyAssignableDescendant::m_d;
        m_saS.m_i -= 1;

        m_diamond.fill();
        m_sptCs.fill();
        m_saaToStS.fill();
        m_saStS.fill();
        m_stS.fill();
        m_mpt.fill();
    }

    [[nodiscard]] auto operator<=>(const SForAllModesTests_Version2&) const = default;

    Diamond<> m_diamond;
    SpecialProcessingType<> m_sptCs;
    SimplyAssignableAlignedToOneSimilarType1<> m_saaToStS;
    SimplyAssignableSimilarType1<> m_saStS;
    SimilarType1<> m_stS;
    ManyPointersType<> m_mpt;

    SimplyAssignable<> m_saS;
    int m_i{ 0 }; // duplicated m_saDs.m_i

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SForAllModesTests_Version0<T>;
    friend DForAllModesTests<T>;
};

template<typename T1>
template<typename T2>
ags_cs::Status SForAllModesTests_Version0<T1>::init(const SForAllModesTests_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    static_cast<SimplyAssignable_Version0<>&>(m_saDs).init(rhs.m_saS);
    m_saDs.m_d = rhs.m_i;
    m_saDs.m_i += 1; // additional conversion

    return ags_cs::Status::NoError;
}

template<typename T1>
template<typename T2>
ags_cs::Status SForAllModesTests_Version2<T1>::init(const SForAllModesTests_Version0<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saS.init(static_cast<const SimplyAssignable_Version0<>&>(rhs.m_saDs));
    m_i = rhs.m_saDs.m_d;
    m_saS.m_i -= 1; // additional conversion

    return ags_cs::Status::NoError;
}

template<typename T1>
template<typename T2>
ags_cs::Status SForAllModesTests_Version2<T1>::init(const DForAllModesTests<T2>& rhs)
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

    return ags_cs::Status::NoError;
}

template<typename T1>
template<typename T2>
ags_cs::Status DForAllModesTests<T1>::init(const SForAllModesTests_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    static_cast<SimplyAssignable<>&>(m_saDs) = rhs.m_saS;
    m_saDs.m_i -= 1; // additional conversion
    m_saDs.m_d = rhs.m_i;

    return ags_cs::Status::NoError;
}

} // namespace tests_csp_interface
