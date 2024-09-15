/**
 * @file common_serialization/tests_csp_interface/structs.h
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

#include <common_serialization/csp_base/csp_base.h>
#include <common_serialization/tests_csp_interface/interface.h>
#include <common_serialization/tests_restricted_structs/tests_restricted_structs.h>

namespace tests_csp_interface
{

template<typename _T>
constexpr void cleanAfterStruct(_T& output)
{ }

template<typename _T = void>
class EmptyType : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<EmptyType<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<EmptyType<>, _T>;
    using empty_type_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0x7f17247f, 0x094b, 0x4cda, 0x9968, 0x66e31b88063c };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    auto operator<=>(const EmptyType&) const = default;
};

#pragma pack(push, 1)

template<typename>
class SimplyAssignableAlignedToOne_Version1;

template<typename _T = void>
class SimplyAssignableAlignedToOne : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOne<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOne<>, _T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0xf2d69dcd, 0x4e24, 0x4c65, 0x9f76, 0xd517be1daccd };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 3, 1, 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename _T2>
    ags_cs::Status init(const SimplyAssignableAlignedToOne_Version1<_T2>& rhs);

    void fill()
    {
        m_x = 3;
        m_y = 4;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableAlignedToOne&) const = default;

    uint8_t m_x{ 0 };
    uint8_t m_y{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableAlignedToOne_Version1<_T>;
};

#pragma pack(pop)

template<typename>
class SimplyAssignable_Version0;

template<typename _T = void>
class SimplyAssignable : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignable<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignable<>, _T>;
    using simply_assignable_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0x81582aae, 0x4a33, 0x4181, 0x8dda, 0xed092c23bccc };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 3;         // latest version among all dependable structs
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 2, 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename _T2>
    ags_cs::Status init(const SimplyAssignable_Version0<_T2>& rhs);

    void fill()
    {
        m_i = 7;
        m_j = 8;

        m_saaToS.fill();
        m_saaToNS.fill();
        m_saNS.fill();

        memcpy(m_arrI32, "123456789012", sizeof(m_arrI32));

        m_arrSaaTos[0].m_x = 48;
        m_arrSaaTos[0].m_y = 60;
        m_arrSaaTos[1].m_x = 49;
        m_arrSaaTos[1].m_y = 61;
        m_arrSaaTos[2].m_x = 50;
        m_arrSaaTos[2].m_y = 62;

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

        m_vx = 46984;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignable&) const = default;

    uint8_t m_i{ 0 };
    uint16_t m_j{ 0 };
    EmptyType<> m_et;
    tests_restricted_structs::EmptyType m_et2;
    SimplyAssignableAlignedToOne<> m_saaToS;
    tests_restricted_structs::SimplyAssignableAlignedToOne m_saaToNS;
    tests_restricted_structs::SimplyAssignable m_saNS;

    uint32_t m_arrI32[3]{ 0 };
    EmptyType<> m_arrEtS[3]{};
    tests_restricted_structs::EmptyType m_arrEtNS[3]{};
    SimplyAssignableAlignedToOne<> m_arrSaaTos[3]{};
    tests_restricted_structs::SimplyAssignableAlignedToOne m_arrSaaToNS[3]{};
    tests_restricted_structs::SimplyAssignable m_arrSaNS[3]{};

    uint64_t m_vx{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignable_Version0<_T>;
};

template<typename>
class SimplyAssignableDescendant_Version0;

template<typename _T = void>
struct SimplyAssignableDescendant : public SimplyAssignable<ags_cs::GetCrtpMainType<SimplyAssignableDescendant<>, _T>>
{
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableDescendant<>, _T>;
    using simply_assignable_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0x81d67474, 0xce11, 0x465e, 0x8558, 0xe10e84939b84 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 2, 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    uint32_t m_d{ 0 };

    template<typename _T2>
    ags_cs::Status init(const SimplyAssignableDescendant_Version0<_T2>& rhs);
    
    [[nodiscard]] operator SimplyAssignable<>& () noexcept
    {
        return  *static_cast<SimplyAssignable<>*>(
                    static_cast<void*>(
                        static_cast<SimplyAssignable<instance_type>*>(this)));
    }

    [[nodiscard]] operator const SimplyAssignable<>& () const noexcept
    {
        return  *static_cast<const SimplyAssignable<>*>(
                    static_cast<const void*>(
                        static_cast<const SimplyAssignable<instance_type>*>(this)));
    }

    void fill()
    {
        SimplyAssignable<instance_type>::fill();
        m_d = this->m_i + 2;
    }
    
    [[nodiscard]] auto operator<=>(const SimplyAssignableDescendant&) const = default;

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableDescendant_Version0<_T>;
};

#pragma pack(push, 1)

template<typename>
class AlwaysSimplyAssignable_Version0;

template<typename _T = void>
class AlwaysSimplyAssignable : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<AlwaysSimplyAssignable<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<AlwaysSimplyAssignable<>, _T>;
    using always_simply_assignable_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0x06c2c5ec, 0x28b1, 0x4b14, 0xa487, 0xc9f6d98fabfd };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 2;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 2, 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename _T2>
    ags_cs::Status init(const AlwaysSimplyAssignable_Version0<_T2>& rhs);

    void fill()
    {
        m_x = 210;
        m_y = 115;
    }

    [[nodiscard]] auto operator<=>(const AlwaysSimplyAssignable&) const = default;

    uint8_t m_x{ 0 };
    uint8_t m_y{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend AlwaysSimplyAssignable_Version0<_T>;
};

#pragma pack(pop)

template<typename>
class SimplyAssignableFixedSize_Version1;

template<typename _T = void>
class SimplyAssignableFixedSize : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableFixedSize<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableFixedSize<>, _T>;
    using simply_assignable_fixed_size_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0x33347d40, 0x89c5, 0x4eb1, 0xbde2, 0xf398118ebc2f };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 2;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 2, 1 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename _T2>
    ags_cs::Status init(const SimplyAssignableFixedSize_Version1<_T2>& rhs);

    void fill()
    {
        m_x = 178;
        m_asa.fill();
        m_arrAsa[0].m_x = 30;
        m_arrAsa[0].m_y = 132;
        m_arrAsa[1].m_x = 88;
        m_arrAsa[1].m_y = 53;
        m_arrAsa[2].m_x = 159;
        m_arrAsa[2].m_y = 227;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableFixedSize&) const = default;

    uint8_t m_x{ 0 };
    AlwaysSimplyAssignable<> m_asa;
    AlwaysSimplyAssignable<> m_arrAsa[3];

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SimplyAssignableFixedSize_Version1<_T>;
};

template<typename _T = void>
class DynamicPolymorphic : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<DynamicPolymorphic<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<DynamicPolymorphic<>, _T>;

    virtual ~DynamicPolymorphic() {}

    static constexpr ags_cs::csp::Id kId{ 0x7df21aa3, 0x9999, 0x4fa7, 0xa34d, 0xd7190e818392 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_o = 11;
        m_dpNS.fill();

        memcpy(m_arrO, "def", sizeof(m_arrO));

        m_arrDpNS->fill();

        m_arrDpNS[1].m_r = 12;
        memcpy(m_arrDpNS[1].m_arrR, "ghi", sizeof(m_arrDpNS[1].m_arrR));
        m_arrDpNS[2].m_r = 13;
        memcpy(m_arrDpNS[2].m_arrR, "jkl", sizeof(m_arrDpNS[2].m_arrR));
    }

    [[nodiscard]] auto operator<=>(const DynamicPolymorphic&) const = default;

    uint8_t m_o{ 0 };
    tests_restricted_structs::DynamicPolymorphic m_dpNS{};

    uint8_t m_arrO[3]{ 0 };
    tests_restricted_structs::DynamicPolymorphic m_arrDpNS[3]{};

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T = void>
class Diamond 
    : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<Diamond<_T>, _T >>
    , public tests_restricted_structs::DiamondEdge1
    , public tests_restricted_structs::DiamondEdge2
{
public:
    using instance_type = ags_cs::GetCrtpMainType<Diamond<_T>, _T>;

    static constexpr ags_cs::csp::Id kId{ 0xcbe00219, 0x483c, 0x4f3d, 0xbe2c, 0x9878c3541d7c };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        tests_restricted_structs::DiamondEdge1::fill();
        tests_restricted_structs::DiamondEdge2::fill();
    }

    // There is a bug in gcc when operator<=> not work properly on "Diamond-based" classes
    //[[nodiscard]] auto operator<=>(const Diamond&) const = default;
    [[nodiscard]] bool operator==(const Diamond& rhs) const
    {
        return m_d0 == rhs.m_d0 && m_d1 == rhs.m_d1 && m_d2 == rhs.m_d2;
    }

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T = void>
class SpecialProcessingType : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SpecialProcessingType<>, _T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SpecialProcessingType<>, _T>;

    static constexpr ags_cs::csp::Id kId{ 0xada5ded5, 0x2568, 0x44ee, 0x961e, 0xf83491911449 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    enum class TEnum : uint64_t
    {
        Val0 = 0,
        ValN = 8192058,
        ValN2 = 2309348230323
    };
    /*
    SpecialProcessingType() = default;

    SpecialProcessingType(const SpecialProcessingType&)
    {
        fill();
    }

    SpecialProcessingType& operator=(const SpecialProcessingType& rhs)
    {
        if (this != &rhs)
            fill();

        return *this;
    }

    SpecialProcessingType(const SpecialProcessingType&& rhs)
    {
        *this = std::move(rhs);
    }

    SpecialProcessingType& operator=(SpecialProcessingType&& rhs) noexcept
    {
        m_vec = std::move(rhs.m_vec);
        m_saaToNS = std::move(rhs.m_saaToNS);
        m_saNS = std::move(rhs.m_saNS);
        m_pVec = rhs.m_pVec;
        rhs.m_pVec = nullptr;
        m_pInt = rhs.m_pInt;
        rhs.m_pInt = nullptr;
        m_ppInt = rhs.m_ppInt;
        rhs.m_ppInt = nullptr;
        m_nullptrInt = rhs.m_nullptrInt; // must be set to nullptr at filling struct function
        rhs.m_nullptrInt = nullptr;

        m_c = rhs.m_c;
        m_sh = rhs.m_sh;
        m_m = rhs.m_m;
        m_tEnum = rhs.m_tEnum;
        m_ll = rhs.m_ll;
        m_float = rhs.m_float;;
        m_double = rhs.m_double;

        return *this;
    }*/

    void fill()
    {
        Diamond ds1;
        ds1.fill();
        m_vec.pushBack(ds1);

        Diamond ds2;
        ds2.m_d0 = 23;
        ds2.m_d0 = 24;
        ds2.m_d0 = 25;

        m_vec.pushBack(ds2);
        m_saaToNS.fill();
        m_saNS.fill();

        m_pVec = new ags_cs::VectorT<Diamond<>>;
        *m_pVec = m_vec;
        m_pInt = new int;
        *const_cast<int*&>(m_pInt) = 64984561;

        m_ppInt = new const int*;
        *m_ppInt = new int;
        **const_cast<int**&>(m_ppInt) = 548505248;

        m_nullptrInt = nullptr;

        m_c = -82;
        m_sh = 48893;
        m_m = ags_cs::csp::context::Message::Data;
        m_tEnum = TEnum::ValN2;
        m_ll = -83242399125;
        m_float = 0.326236329032f;
        m_double = -13523623623.25;
    }

    [[nodiscard]] bool operator==(const SpecialProcessingType& rhs) const noexcept
    {
        return m_vec == rhs.m_vec
            && m_saaToNS == rhs.m_saaToNS
            && m_saNS == rhs.m_saNS
            && *m_pVec == *rhs.m_pVec   // this class only for testing proposes, so there is no redundant nullptr checks
            && **m_ppInt == **rhs.m_ppInt
            && m_nullptrInt == rhs.m_nullptrInt
            && m_c == rhs.m_c
            && m_sh == rhs.m_sh
            && m_tEnum == rhs.m_tEnum
            && m_float == rhs.m_float
            && m_double == rhs.m_double;
    }

    void clean()
    {
        delete m_pVec;
        delete m_pInt;
        delete* m_ppInt;
        delete m_ppInt;
    }

    ags_cs::VectorT<Diamond<>> m_vec;
    tests_restricted_structs::SimplyAssignableAlignedToOne m_saaToNS;
    tests_restricted_structs::SimplyAssignable m_saNS;
    ags_cs::VectorT<Diamond<>>* m_pVec{ nullptr };
    const int* m_pInt{ nullptr };
    const int** m_ppInt{ nullptr };
    const int* m_nullptrInt{ reinterpret_cast<const int*>(-1) }; // must be set to nullptr at filling struct function

    char m_c{ 0 };
    unsigned short m_sh{ 0 };
    ags_cs::csp::context::Message m_m{ ags_cs::csp::context::Message::Status };
    TEnum m_tEnum{ TEnum::Val0 };
    int64_t m_ll{ 0 };
    float m_float{ 0 };
    double m_double{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
};

#pragma pack(push, 1)

template<typename>
class SimplyAssignableAlignedToOneSimilarType2;

template<typename _T = void>
class SimplyAssignableAlignedToOneSimilarType1 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOneSimilarType1<>, _T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOneSimilarType1<>, _T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0xc009d078, 0xbd81, 0x41ae, 0xb303, 0xb6a361922373 }; // id is same as in SimplyAssignableAlignedToOneSimilarType2 (need for tests)
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_j = 99;
        m_k = 15456;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableAlignedToOneSimilarType1&) const = default;

    SimplyAssignableAlignedToOneSimilarType1<_T>& operator=(const SimplyAssignableAlignedToOneSimilarType2<_T>& rhs) noexcept;

    char m_j{ 0 };
    int m_k{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T = void>
class SimplyAssignableAlignedToOneSimilarType2 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOneSimilarType2<>, _T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableAlignedToOneSimilarType2<>, _T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0xc009d078, 0xbd81, 0x41ae, 0xb303, 0xb6a361922373 }; // hash is same as in AlignedToOneSimilarType1Serializable (need for tests)
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_j = 99;
        m_k = 15456;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableAlignedToOneSimilarType2&) const = default;

    SimplyAssignableAlignedToOneSimilarType2<_T>& operator=(const SimplyAssignableAlignedToOneSimilarType1<_T>& rhs) noexcept
    {
        m_j = rhs.m_j;
        m_k = static_cast<short>(rhs.m_k);

        return *this;
    }

    char m_j{ 0 };
    short m_k{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T>
SimplyAssignableAlignedToOneSimilarType1<_T>& SimplyAssignableAlignedToOneSimilarType1<_T>::operator=(const SimplyAssignableAlignedToOneSimilarType2<_T>& rhs) noexcept
{
    m_j = rhs.m_j;
    m_k = static_cast<int>(rhs.m_k);

    return *this;
}

#pragma pack(pop)

template<typename>
class SimplyAssignableSimilarType2;

template<typename _T = void>
class SimplyAssignableSimilarType1 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableSimilarType1<>, _T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableSimilarType1<>, _T>;
    using simply_assignable_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0xab625590, 0xcd29, 0x43d0, 0xadfd, 0xc59273696583 }; // id is same as in SimplyAssignableSimilarType2 (need for tests)
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_j = 101;
        m_k = 8374;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableSimilarType1&) const = default;

    SimplyAssignableSimilarType1<_T>& operator=(const SimplyAssignableSimilarType2<_T>& rhs) noexcept;

    char m_j{ 0 };
    int m_k{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T = void>
class SimplyAssignableSimilarType2 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableSimilarType2<>, _T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableSimilarType2<>, _T>;
    using simply_assignable_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0xab625590, 0xcd29, 0x43d0, 0xadfd, 0xc59273696583 }; // id is same as in SimplyAssignableSimilarType1 (need for tests)
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_j = 101;
        m_k = 8374;
    }

    [[nodiscard]] auto operator<=>(const SimplyAssignableSimilarType2&) const = default;

    SimplyAssignableSimilarType2<_T>& operator=(const SimplyAssignableSimilarType1<_T>& rhs) noexcept
    {
        m_j = rhs.m_j;
        m_k = static_cast<short>(rhs.m_k);

        return *this;
    }

    char m_j{ 0 };
    short m_k{ 0 };

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T>
SimplyAssignableSimilarType1<_T>& SimplyAssignableSimilarType1<_T>::operator=(const SimplyAssignableSimilarType2<_T>& rhs) noexcept
{
    m_j = rhs.m_j;
    m_k = static_cast<int>(rhs.m_k);

    return *this;
}

template<typename>
class SimilarType2;

template<typename _T = void>
class SimilarType1 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimilarType1<>, _T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimilarType1<>, _T>;

    static constexpr ags_cs::csp::Id kId{ 0x0bcf5909, 0xad8a, 0x48f2, 0xb1fd, 0xe9b781ce0b2d }; // hash is same as in SimilarType2 (need for tests)
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_j = 102;
        m_k = 9951;

        m_arrL[0] = 34;
        m_arrL[1] = 44;
        m_arrL[2] = 54;
        m_arrL[3] = 64;
        m_arrL[4] = 74;

        m_sasTs[0].m_j = 105;
        m_sasTs[0].m_k = 11123;
        m_sasTs[1].m_j = 100;
        m_sasTs[1].m_k = 20110;
        m_sasTs[2].m_j = 105;
        m_sasTs[2].m_k = 25548;

        m_saaToSts[0].m_j = 21;
        m_saaToSts[0].m_k = 11923;
        m_saaToSts[1].m_j = 85;
        m_saaToSts[1].m_k = 31002;
        m_saaToSts[2].m_j = 54;
        m_saaToSts[2].m_k = 4563;
    }

    [[nodiscard]] auto operator<=>(const SimilarType1&) const = default;

    SimilarType1<_T>& operator=(const SimilarType2<_T>& rhs) noexcept;

    char m_j{ 0 };
    int m_k{ 0 };
    uint8_t m_arrL[5]{ 0 };
    SimplyAssignableSimilarType1<> m_sasTs[3];
    SimplyAssignableAlignedToOneSimilarType1<> m_saaToSts[3];

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T = void>
class SimilarType2 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimilarType2<>, _T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimilarType2<>, _T>;

    static constexpr ags_cs::csp::Id kId{ 0x0bcf5909, 0xad8a, 0x48f2, 0xb1fd, 0xe9b781ce0b2d }; // id is same as in SimilarType1 (need for tests)
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_j = 102;
        m_k = 9951;

        m_arrL[0] = 34;
        m_arrL[1] = 44;
        m_arrL[2] = 54;
        m_arrL[3] = 64;
        m_arrL[4] = 74;

        m_sasTs[0].m_j = 105;
        m_sasTs[0].m_k = 11123;
        m_sasTs[1].m_j = 100;
        m_sasTs[1].m_k = 20110;
        m_sasTs[2].m_j = 105;
        m_sasTs[2].m_k = 25548;

        m_saaToSts[0].m_j = 21;
        m_saaToSts[0].m_k = 11923;
        m_saaToSts[1].m_j = 85;
        m_saaToSts[1].m_k = 31002;
        m_saaToSts[2].m_j = 54;
        m_saaToSts[2].m_k = 4563;
    }

    [[nodiscard]] auto operator<=>(const SimilarType2&) const = default;

    SimilarType2<_T>& operator=(const SimilarType1<_T>& rhs) noexcept
    {
        m_j = rhs.m_j;
        m_k = static_cast<short>(rhs.m_k);

        for (size_t i = 0; i < std::size(m_arrL); ++i)
            m_arrL[i] = rhs.m_arrL[i];

        for (size_t i = 0; i < std::size(m_sasTs); ++i)
            m_sasTs[i] = rhs.m_sasTs[i];

        for (size_t i = 0; i < std::size(m_saaToSts); ++i)
            m_saaToSts[i] = rhs.m_saaToSts[i];

        return *this;
    }

    char m_j{ 0 };
    short m_k{ 0 };
    uint32_t m_arrL[5]{ 0 };
    SimplyAssignableSimilarType2<> m_sasTs[3];
    SimplyAssignableAlignedToOneSimilarType2<> m_saaToSts[3];

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T>
SimilarType1<_T>& SimilarType1<_T>::operator=(const SimilarType2<_T>& rhs) noexcept
{
    m_j = rhs.m_j;
    m_k = static_cast<int>(rhs.m_k);

    for (size_t i = 0; i < std::size(m_arrL); ++i)
        m_arrL[i] = rhs.m_arrL[i];

    for (size_t i = 0; i < std::size(m_sasTs); ++i)
        m_sasTs[i] = rhs.m_sasTs[i];

    for (size_t i = 0; i < std::size(m_saaToSts); ++i)
        m_saaToSts[i] = rhs.m_saaToSts[i];

    return *this;
}

template<typename _T = void>
class ManyPointersType : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<ManyPointersType<>, _T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<ManyPointersType<>, _T>;

    static constexpr ags_cs::csp::Id kId{ 0xc3d3dfdf, 0x27a2, 0x47e2, 0xbddd, 0x671c180db011 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }
    /*
    ManyPointersType() = default;

    ManyPointersType(const ManyPointersType&)
    {
        fill();
    }

    ManyPointersType& operator=(const ManyPointersType& rhs)
    {
        if (this != &rhs)
            fill();

        return *this;
    }

    ManyPointersType(const ManyPointersType&& rhs) noexcept
    {
        *this = std::move(rhs);
    }

    ManyPointersType& operator=(ManyPointersType&& rhs) noexcept
    {
        m_vec = std::move(rhs);
        m_pInt = rhs.m_pInt;
        rhs.m_pInt = nullptr;
        m_vecRecursive = std::move(rhs.m_vecRecursive);
        m_pVec = rhs.m_pVec;
        m_rtSpec1 = rhs.m_rtSpec1;
        m_rtSpec2 = rhs.m_rtSpec2;
        memcpy(m_intArr, m_pInt, sizeof(m_intArr));
        m_ppInt = &m_pInt;

        return *this;
    }*/

    void fill()
    {
        ags_cs::RawVectorT<int*> vec;
        // It must be raw pointer, which will be deleted on test cleanup
        m_pInt = new int[3] { 1, 2, 3};
        vec.pushBack(m_pInt);
        vec.pushBack(m_pInt + 1);
        vec.pushBack(m_pInt + 2);

        m_vec = vec;
        m_vecRecursive.pushBack(&m_vec);
        m_vecRecursive.pushBack(nullptr);
        m_pVec = &m_vec;

        m_rtSpec1.m_pAny = &m_rtSpec2;
        m_rtSpec2.m_pI = m_pInt;
        m_rtSpec2.m_pAny = &m_rtSpec1;
        m_rtSpec2.m_pNext = &m_rtSpec2;

        memcpy(m_intArr, m_pInt, sizeof(m_intArr));
        m_ppInt = &m_pInt;
    }

    [[nodiscard]] bool operator==(const ManyPointersType& rhs) const noexcept
    {
        return
               *m_vec[0] == *rhs.m_vec[0]
            && *m_vec[1] == *rhs.m_vec[1]
            && *m_vec[2] == *rhs.m_vec[2]
            && *(*m_vecRecursive[0])[0] == *(*rhs.m_vecRecursive[0])[0]
            && *(*m_vecRecursive[0])[1] == *(*rhs.m_vecRecursive[0])[1]
            && *(*m_vecRecursive[0])[2] == *(*rhs.m_vecRecursive[0])[2]
            && m_vecRecursive[1] == rhs.m_vecRecursive[1] // must by nullptr
            && *(*m_pVec)[0] == *(*rhs.m_pVec)[0]
            && *(*m_pVec)[1] == *(*rhs.m_pVec)[1]
            && *(*m_pVec)[2] == *(*rhs.m_pVec)[2]
            && *m_rtSpec1.m_pAny->m_pI == *rhs.m_rtSpec2.m_pI
            && *m_rtSpec1.m_pAny->m_pAny->m_pAny->m_pI == *rhs.m_rtSpec2.m_pI
            && *m_rtSpec1.m_pAny->m_pNext->m_pI == *rhs.m_rtSpec2.m_pI

            && *m_rtSpec2.m_pI == *rhs.m_rtSpec2.m_pI
            && *m_rtSpec2.m_pAny->m_pAny->m_pI == *rhs.m_rtSpec2.m_pI
            && *m_rtSpec2.m_pNext->m_pI == *rhs.m_rtSpec2.m_pI

            && *m_pInt == *rhs.m_pInt
            && memcmp(m_intArr, rhs.m_intArr, sizeof(m_intArr)) == 0
            && **m_ppInt == **rhs.m_ppInt
            && m_nullptrInt == nullptr;
    }

    void clean()
    {
        delete[] m_pInt;
    }

    ags_cs::RawVectorT<int*> m_vec;
    ags_cs::RawVectorT<ags_cs::RawVectorT<int*>*> m_vecRecursive;
    ags_cs::RawVectorT<int*>* m_pVec;

    tests_restricted_structs::RecursiveTestSpecial1 m_rtSpec1;
    tests_restricted_structs::RecursiveTestSpecial2 m_rtSpec2;

    int* m_pInt{ nullptr };
    int m_intArr[3]{ 0 };
    int** m_ppInt{ nullptr };
    int* m_nullptrInt{ nullptr };

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename> 
class SForAllModesTests_Version2;

// DForAllModesTests shall be used in deserialization of SForAllModesTests, but only with sizeOfIntegersMayBeNotEqual flag set
template<typename _T = void>
class DForAllModesTests : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<DForAllModesTests<>, _T >>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<DForAllModesTests<>, _T>;

    static constexpr ags_cs::csp::Id kId{ 0x953e9f24, 0xc725, 0x4903, 0x8219, 0xf37d084ef557 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 3, 2, 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename _T2>
    ags_cs::Status init(const SForAllModesTests_Version2<_T2>& rhs);

    void fill()
    {
        m_saDs.fill();
        m_saDs.m_i -= 2;                 // == SimplyAssignable::m_i - 2;

        m_diamond.fill();
        m_sptCs.fill();
        m_saaToStS.fill();
        m_saStS.fill();
        m_stS.fill();
        m_mpt.fill();
    }

    [[nodiscard]] auto operator<=>(const DForAllModesTests&) const = default;

    void clean()
    {
        m_sptCs.clean();
        m_mpt.clean();
    }

    SimplyAssignableDescendant<> m_saDs;
    Diamond<> m_diamond;
    SpecialProcessingType<> m_sptCs;
    SimplyAssignableAlignedToOneSimilarType2<> m_saaToStS;
    SimplyAssignableSimilarType2<> m_saStS;
    SimilarType2<> m_stS;
    ManyPointersType<> m_mpt;

    friend ags_cs::csp::processing::data::BodyProcessor;
    friend SForAllModesTests_Version2<_T>;
};

template<typename _T = void>
class SimplyAssignableWithoutSerializationFunctions : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<SimplyAssignableWithoutSerializationFunctions<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<SimplyAssignableWithoutSerializationFunctions<>, _T>;
    using simply_assignable_tag = std::true_type;

    static constexpr ags_cs::csp::Id kId{ 0x4e613dd4, 0xf408, 0x4f5b, 0xa18f, 0xec9e8f4c86ce };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_i = 24801;
    }

    uint32_t m_i{ 0 };

    [[nodiscard]] auto operator<=>(const SimplyAssignableWithoutSerializationFunctions&) const = default;

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T = void>
class ContainSimplyAssignableWithoutSerializationFunctions : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<ContainSimplyAssignableWithoutSerializationFunctions<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<ContainSimplyAssignableWithoutSerializationFunctions<>, _T>;

    static constexpr ags_cs::csp::Id kId{ 0xf2b9a015, 0x1ec1, 0x4b06, 0xa313, 0x158b97af8fa4 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    void fill()
    {
        m_sawsf.fill();
        m_npfSawsf.fill();
    }

    [[nodiscard]] auto operator<=>(const ContainSimplyAssignableWithoutSerializationFunctions&) const = default;

    SimplyAssignableWithoutSerializationFunctions<> m_sawsf;
    tests_restricted_structs::SimplyAssignableWithoutSerializationFunctions m_npfSawsf;

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T>
struct BigStructs
{
    [[nodiscard]] auto operator<=>(const BigStructs&) const = default;

    ags_cs::VectorT<_T> m_vector;
};

template<typename _T = void>
class ContainBigStructs1 : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<ContainBigStructs1<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<ContainBigStructs1<>, _T>;

    static constexpr ags_cs::csp::Id kId{ 0xfa3404da, 0x0af9, 0x4632, 0xb3b5, 0x02d82446ef24 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    [[nodiscard]] auto operator<=>(const ContainBigStructs1&) const = default;

    BigStructs<SimplyAssignableAlignedToOne<>> m_big1;
    BigStructs<SimplyAssignable<>> m_big2;
    BigStructs<SimplyAssignableDescendant<>> m_big3;
    BigStructs<AlwaysSimplyAssignable<>> m_big4;
    BigStructs<SimplyAssignableFixedSize<>> m_big5;
    BigStructs<DynamicPolymorphic<>> m_big6;
    BigStructs<Diamond<>> m_big7;

    void clean()
    {

    }

    friend ags_cs::csp::processing::data::BodyProcessor;
};

template<typename _T = void>
class ContainBigStructs2ForAllModes : public ags_cs::csp::ISerializable<ags_cs::GetCrtpMainType<ContainBigStructs2ForAllModes<>, _T>>
{
public:
    using instance_type = ags_cs::GetCrtpMainType<ContainBigStructs2ForAllModes<>, _T>;

    static constexpr ags_cs::csp::Id kId{ 0xb62add3d, 0x3695, 0x4432, 0xa8b4, 0x5a799f4a7796 };
    static constexpr ags_cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr ags_cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const ags_cs::csp::Interface& getInterface() noexcept { return properties; }

    [[nodiscard]] auto operator<=>(const ContainBigStructs2ForAllModes&) const = default;

    BigStructs<DForAllModesTests<>> m_big;

    friend ags_cs::csp::processing::data::BodyProcessor;
};

} // namespace tests_csp_interface


