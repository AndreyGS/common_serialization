/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/include/interface_for_test/SpecialTypesSerializableLegacy.h
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

namespace interface_for_test
{

namespace cs = common_serialization;

template<typename T = cs::Dummy>
class SimplyAssignableAlignedToOne_Version0 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableAlignedToOne_Version0<T>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableAlignedToOne_Version0<T>, T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr cs::csp::Id kId = SimplyAssignableAlignedToOne<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    cs::Status init(const SimplyAssignableAlignedToOne_Version1<T2>& rhs);

    [[nodiscard]] auto operator<=>(const SimplyAssignableAlignedToOne_Version0&) const = default;

    not_part_of_interfaces::TwoInts m_ti{ 0 };

    friend cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableAlignedToOne_Version1<T>;
};

template<typename T = cs::Dummy>
class SimplyAssignable_Version0 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignable_Version0<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignable_Version0<>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId = SimplyAssignable<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;         // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    cs::Status init(const SimplyAssignable<T2>& rhs);

    [[nodiscard]] auto operator<=>(const SimplyAssignable_Version0&) const = default;

    uint8_t m_i{ 0 };
    uint16_t m_j{ 0 };
    EmptyType<> m_et;
    not_part_of_interfaces::EmptyType m_et2;
    SimplyAssignableAlignedToOne_Version0<> m_saaToS;
    not_part_of_interfaces::SimplyAssignableAlignedToOne m_saaToNS;
    not_part_of_interfaces::SimplyAssignable m_saNS;

    uint32_t m_arrI32[3]{ 0 };
    EmptyType<> m_arrEtS[3]{};
    not_part_of_interfaces::EmptyType m_arrEtNS[3]{};
    SimplyAssignableAlignedToOne_Version0<> m_arrSaaTos[3]{};
    not_part_of_interfaces::SimplyAssignableAlignedToOne m_arrSaaToNS[3]{};
    not_part_of_interfaces::SimplyAssignable m_arrSaNS[3]{};

    uint32_t m_vt{ 0 };

    friend cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignable<T>;
};

template<typename T1>
template<typename T2>
cs::Status SimplyAssignable_Version0<T1>::init(const SimplyAssignable<T2>& rhs)
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

    return cs::Status::NoError;
}

template<typename T1>
template<typename T2>
cs::Status SimplyAssignable<T1>::init(const SimplyAssignable_Version0<T2>& rhs)
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

    return cs::Status::NoError;
}

template<typename T = cs::Dummy>
struct SimplyAssignableDescendant_Version0 : public SimplyAssignable_Version0<cs::GetCrtpMainType<SimplyAssignableDescendant_Version0<>, T>>
{
    using instance_type = cs::GetCrtpMainType<SimplyAssignableDescendant_Version0<>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId = SimplyAssignableDescendant<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    uint32_t m_d{ 0 };

    template<typename T2>
    cs::Status init(const SimplyAssignableDescendant<T2>& rhs);

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

    [[nodiscard]] auto operator<=>(const SimplyAssignableDescendant_Version0&) const = default;

    friend cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableDescendant<T>;
};


template<typename T1>
template<typename T2>
cs::Status SimplyAssignableDescendant_Version0<T1>::init(const SimplyAssignableDescendant<T2>& rhs)
{
    static_cast<SimplyAssignable_Version0<T1>&>(*this).init(rhs);
    m_d = rhs.m_d;

    return cs::Status::NoError;
}

template<typename T1>
template<typename T2>
cs::Status SimplyAssignableDescendant<T1>::init(const SimplyAssignableDescendant_Version0<T2>& rhs)
{
    static_cast<SimplyAssignable<T2>&>(*this).init(rhs);
    m_d = rhs.m_d;

    return cs::Status::NoError;
}

#pragma pack(push, 1)

template<typename T = cs::Dummy>
class AlwaysSimplyAssignable_Version0 : public cs::csp::ISerializable<cs::GetCrtpMainType<AlwaysSimplyAssignable_Version0<T>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<AlwaysSimplyAssignable_Version0<T>, T>;
    using always_simply_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId = AlwaysSimplyAssignable<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    cs::Status init(const AlwaysSimplyAssignable<T2>& rhs);

    [[nodiscard]] auto operator<=>(const AlwaysSimplyAssignable_Version0&) const = default;

    uint16_t m_xx{ 0 };
    uint16_t m_yy{ 0 };

    friend cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableAlignedToOne<T>;
};

#pragma pack(pop)

template<typename T1>
template<typename T2>
cs::Status AlwaysSimplyAssignable_Version0<T1>::init(const AlwaysSimplyAssignable<T2>& rhs)
{
    m_xx = rhs.m_x + 1;
    m_yy = rhs.m_y + 2;

    return cs::Status::NoError;
}

template<typename T1>
template<typename T2>
cs::Status AlwaysSimplyAssignable<T1>::init(const AlwaysSimplyAssignable_Version0<T2>& rhs)
{
    m_x = rhs.m_xx - 1;
    m_y = rhs.m_yy - 2;

    return cs::Status::NoError;
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
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    cs::Status init(const SForAllModesTests_Version2<T2>& rhs);

    [[nodiscard]] auto operator<=>(const SForAllModesTests_Version0&) const = default;

    SimplyAssignableDescendant_Version0<> m_saDs;
    Diamond<> m_diamond;
    SpecialProcessingType<> m_sptCs;
    SimplyAssignableAlignedToOneSimilarType1<> m_saaToStS;
    SimplyAssignableSimilarType1<> m_saStS;
    SimilarType1<> m_stS;
    ManyPointersType<> m_mpt;

    friend cs::csp::processing::data::BodyProcessor;
    friend SForAllModesTests_Version2<T>;
};

template<typename T = cs::Dummy>
class SimplyAssignableAlignedToOne_Version1 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableAlignedToOne_Version1<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableAlignedToOne_Version1<>, T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr cs::csp::Id kId = SimplyAssignableAlignedToOne<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 1, 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    cs::Status init(const SimplyAssignableAlignedToOne_Version0<T2>& rhs)
    {
        m_x = rhs.m_ti.x;
        m_y = rhs.m_ti.y;

        return cs::Status::NoError;
    }

    template<typename T2>
    cs::Status init(const SimplyAssignableAlignedToOne<T2>& rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;

        return cs::Status::NoError;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableAlignedToOne_Version1&) const = default;

    uint8_t m_x{ 0 };
    uint16_t m_y{ 0 };

    friend cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableAlignedToOne_Version0<T>;
    friend SimplyAssignableAlignedToOne<T>;
};

template<typename T1>
template<typename T2>
cs::Status SimplyAssignableAlignedToOne_Version0<T1>::init(const SimplyAssignableAlignedToOne_Version1<T2>& rhs)
{
    m_ti.x = rhs.m_x;
    m_ti.y = rhs.m_y;

    return cs::Status::NoError;
}

template<typename T1>
template<typename T2>
cs::Status SimplyAssignableAlignedToOne<T1>::init(const SimplyAssignableAlignedToOne_Version1<T2>& rhs)
{
    m_x = rhs.m_x;
    m_y = rhs.m_y;

    return cs::Status::NoError;
}

template<typename T = cs::Dummy>
class SimplyAssignableFixedSize_Version1 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableFixedSize_Version1<T>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableFixedSize_Version1<T>, T>;
    using simply_assignable_fixed_size_tag = std::true_type;

    static constexpr cs::csp::Id kId = SimplyAssignableFixedSize<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 1 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    cs::Status init(const SimplyAssignableFixedSize<T2>& rhs);

    [[nodiscard]] auto operator<=>(const SimplyAssignableFixedSize_Version1&) const = default;

    uint16_t m_xx{ 0 };
    AlwaysSimplyAssignable_Version0<> m_asa;
    AlwaysSimplyAssignable_Version0<> m_arrAsa[3];

    friend cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableFixedSize<T>;
};

template<typename T1>
template<typename T2>
cs::Status SimplyAssignableFixedSize_Version1<T1>::init(const SimplyAssignableFixedSize<T2>& rhs)
{
    m_xx = rhs.m_x + 1;
    m_asa.init(rhs.m_asa);

    for (size_t i = 0; i < 3; ++i)
        m_arrAsa[i].init(rhs.m_arrAsa[i]);

    return cs::Status::NoError;
}

template<typename T1>
template<typename T2>
cs::Status SimplyAssignableFixedSize<T1>::init(const SimplyAssignableFixedSize_Version1<T2>& rhs)
{
    m_x = rhs.m_xx - 1;
    m_asa.init(rhs.m_asa);

    for (size_t i = 0; i < 3; ++i)
        m_arrAsa[i].init(rhs.m_arrAsa[i]);

    return cs::Status::NoError;
}

template<typename T = cs::Dummy>
class SForAllModesTests_Version2 : public cs::csp::ISerializable<cs::GetCrtpMainType<SForAllModesTests_Version2<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SForAllModesTests_Version2<>, T>;

    static constexpr cs::csp::Id kId = DForAllModesTests<>::getId();
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 2;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 2, 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    cs::Status init(const SForAllModesTests_Version0<T2>& rhs);
    template<typename T2>
    cs::Status init(const DForAllModesTests<T2>& rhs);

    [[nodiscard]] auto operator<=>(const SForAllModesTests_Version2&) const = default;

    Diamond<> m_diamond;
    SpecialProcessingType<> m_sptCs;
    SimplyAssignableAlignedToOneSimilarType1<> m_saaToStS;
    SimplyAssignableSimilarType1<> m_saStS;
    SimilarType1<> m_stS;
    ManyPointersType<> m_mpt;

    SimplyAssignable<> m_saS;
    int m_i{ 0 }; // duplicated m_saDs.m_i

    friend cs::csp::processing::data::BodyProcessor;
    friend SForAllModesTests_Version0<T>;
    friend DForAllModesTests<T>;
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

    static_cast<SimplyAssignable_Version0<>&>(m_saDs).init(rhs.m_saS);
    m_saDs.m_d = rhs.m_i;
    m_saDs.m_i += 1; // additional conversion

    return cs::Status::NoError;
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

    m_saS.init(static_cast<const SimplyAssignable_Version0<>&>(rhs.m_saDs));
    m_i = rhs.m_saDs.m_d;
    m_saS.m_i -= 1; // additional conversion

    return cs::Status::NoError;
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

    return cs::Status::NoError;
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

    static_cast<SimplyAssignable<>&>(m_saDs) = rhs.m_saS;
    m_saDs.m_i -= 1; // additional conversion
    m_saDs.m_d = rhs.m_i;

    return cs::Status::NoError;
}

} // namespace interface_for_test
