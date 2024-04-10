/**
 * @file UnitTests/SerializableStructs/InterfaceForTest/include/interface_for_test/Structs.h
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

template<typename T = cs::Dummy>
class EmptyType : public cs::csp::ISerializable<cs::GetCrtpMainType<EmptyType<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<EmptyType<>, T>;
    using empty_type_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0x7f17247f, 0x094b, 0x4cda, 0x9968, 0x66e31b88063c };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }
};

#pragma pack(push, 1)

template<typename>
class SimplyAssignableAlignedToOne_Version1;

template<typename T = cs::Dummy>
class SimplyAssignableAlignedToOne : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableAlignedToOne<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableAlignedToOne<>, T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0xf2d69dcd, 0x4e24, 0x4c65, 0x9f76, 0xd517be1daccd };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 3, 1, 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    SimplyAssignableAlignedToOne() { }
    template<typename T2>
    cs::Status init(const SimplyAssignableAlignedToOne_Version1<T2>& rhs);

    SimplyAssignableAlignedToOne& operator=(const SimplyAssignableAlignedToOne& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        m_x = rhs.m_x, m_y = rhs.m_y;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimplyAssignableAlignedToOne& rhs) const noexcept
    {
        return m_x == rhs.m_x && m_y == rhs.m_y;
    }

    uint8_t m_x{ 0 };
    uint8_t m_y{ 0 };

    friend cs::csp::processing::BodyProcessor;
    friend SimplyAssignableAlignedToOne_Version1<T>;
};

#pragma pack(pop)

template<typename>
class SimplyAssignable_Version0;

template<typename T = cs::Dummy>
class SimplyAssignable : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignable<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignable<>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0x81582aae, 0x4a33, 0x4181, 0x8dda, 0xed092c23bccc };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 3;         // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 2, 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    cs::Status init(const SimplyAssignable_Version0<T2>& rhs);

    SimplyAssignable& operator=(const SimplyAssignable<>& rhs) noexcept
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
            m_arrSaaTos[i].m_x = rhs.m_arrSaaTos[i].m_x;
            m_arrSaaTos[i].m_y = rhs.m_arrSaaTos[i].m_y;
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

        m_vx = rhs.m_vx;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimplyAssignable& rhs)                  const noexcept
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

            && m_vx == rhs.m_vx
            ;
    }

    uint8_t m_i{ 0 };
    uint16_t m_j{ 0 };
    EmptyType<> m_et;
    not_part_of_interfaces::EmptyType m_et2;
    SimplyAssignableAlignedToOne<> m_saaToS;
    not_part_of_interfaces::SimplyAssignableAlignedToOne m_saaToNS;
    not_part_of_interfaces::SimplyAssignable m_saNS;

    uint32_t m_arrI32[3]{ 0 };
    EmptyType<> m_arrEtS[3]{};
    not_part_of_interfaces::EmptyType m_arrEtNS[3]{};
    SimplyAssignableAlignedToOne<> m_arrSaaTos[3]{};
    not_part_of_interfaces::SimplyAssignableAlignedToOne m_arrSaaToNS[3]{};
    not_part_of_interfaces::SimplyAssignable m_arrSaNS[3]{};

    uint64_t m_vx{ 0 };

    friend cs::csp::processing::BodyProcessor;
    friend SimplyAssignable_Version0<T>;
};

template<typename>
class SimplyAssignableDescendant_Version0;

template<typename T = cs::Dummy>
struct SimplyAssignableDescendant : public SimplyAssignable<cs::GetCrtpMainType<SimplyAssignableDescendant<>, T>>
{
    using instance_type = cs::GetCrtpMainType<SimplyAssignableDescendant<>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0x81d67474, 0xce11, 0x465e, 0x8558, 0xe10e84939b84 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 2, 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    uint32_t m_d{ 0 };

    template<typename T2>
    cs::Status init(const SimplyAssignableDescendant_Version0<T2>& rhs);

    SimplyAssignableDescendant& operator=(const SimplyAssignableDescendant<>& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        SimplyAssignable<>::operator=(rhs);
        m_d = rhs.m_d;

        return *this;
    }
    
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
    
    [[nodiscard]] bool operator==(const SimplyAssignableDescendant& rhs) const noexcept
    {
        return m_d == rhs.m_d && SimplyAssignable<instance_type>::operator==(rhs);
    }

    friend cs::csp::processing::BodyProcessor;
    friend SimplyAssignableDescendant_Version0<T>;
};

#pragma pack(push, 1)

template<typename>
class AlwaysSimplyAssignable_Version0;

template<typename T = cs::Dummy>
class AlwaysSimplyAssignable : public cs::csp::ISerializable<cs::GetCrtpMainType<AlwaysSimplyAssignable<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<AlwaysSimplyAssignable<>, T>;
    using always_simply_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0x06c2c5ec, 0x28b1, 0x4b14, 0xa487, 0xc9f6d98fabfd };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 2;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 2, 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    AlwaysSimplyAssignable() { }
    template<typename T2>
    cs::Status init(const AlwaysSimplyAssignable_Version0<T2>& rhs);

    AlwaysSimplyAssignable& operator=(const AlwaysSimplyAssignable& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        m_x = rhs.m_x, m_y = rhs.m_y;

        return *this;
    }

    [[nodiscard]] bool operator==(const AlwaysSimplyAssignable& rhs) const noexcept
    {
        return m_x == rhs.m_x && m_y == rhs.m_y;
    }

    uint8_t m_x{ 0 };
    uint8_t m_y{ 0 };

    friend cs::csp::processing::BodyProcessor;
    friend AlwaysSimplyAssignable_Version0<T>;
};

#pragma pack(pop)

template<typename>
class SimplyAssignableFixedSize_Version1;

template<typename T = cs::Dummy>
class SimplyAssignableFixedSize : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableFixedSize<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableFixedSize<>, T>;
    using simply_assignable_fixed_size_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0x33347d40, 0x89c5, 0x4eb1, 0xbde2, 0xf398118ebc2f };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 2;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 2, 1 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    SimplyAssignableFixedSize() { }
    template<typename T2>
    cs::Status init(const SimplyAssignableFixedSize_Version1<T2>& rhs);

    SimplyAssignableFixedSize& operator=(const SimplyAssignableFixedSize& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        m_x = rhs.m_x;
        m_asa = rhs.m_asa;

        for (size_t i = 0; i < 3; ++i)
            m_arrAsa[i] = rhs.m_arrAsa[i];

        return *this;
    }

    [[nodiscard]] bool operator==(const SimplyAssignableFixedSize& rhs) const noexcept
    {
        for (size_t i = 0; i < 3; ++i)
            if (m_arrAsa[i] != rhs.m_arrAsa[i])
                return false;

        return m_x == rhs.m_x && m_asa == rhs.m_asa;
    }

    uint8_t m_x{ 0 };
    AlwaysSimplyAssignable<> m_asa;

    AlwaysSimplyAssignable<> m_arrAsa[3];

    friend cs::csp::processing::BodyProcessor;
    friend SimplyAssignableFixedSize_Version1<T>;
};

template<typename T = cs::Dummy>
class DynamicPolymorphic : public cs::csp::ISerializable<cs::GetCrtpMainType<DynamicPolymorphic<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<DynamicPolymorphic<>, T>;

    virtual ~DynamicPolymorphic() {}

    static constexpr cs::csp::Id kId{ 0x7df21aa3, 0x9999, 0x4fa7, 0xa34d, 0xd7190e818392 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    [[nodiscard]] bool operator==(const DynamicPolymorphic& rhs) const noexcept
    {
        return 
               m_o == rhs.m_o 
            && memcmp(m_arrO, rhs.m_arrO, sizeof(m_arrO)) == 0 
            && m_dpNS == rhs.m_dpNS 
            && m_arrDpNS[0] == rhs.m_arrDpNS[0]
            && m_arrDpNS[1] == rhs.m_arrDpNS[1]
            && m_arrDpNS[2] == rhs.m_arrDpNS[2];
    }

    uint8_t m_o{ 0 };
    not_part_of_interfaces::DynamicPolymorphic m_dpNS{};

    uint8_t m_arrO[3]{ 0 };
    not_part_of_interfaces::DynamicPolymorphic m_arrDpNS[3]{};

    friend cs::csp::processing::BodyProcessor;
};

template<typename T = cs::Dummy>
class Diamond 
    : public cs::csp::ISerializable<cs::GetCrtpMainType<Diamond<T>, T >>
    , public not_part_of_interfaces::DiamondEdge1
    , public not_part_of_interfaces::DiamondEdge2
{
public:
    using instance_type = cs::GetCrtpMainType<Diamond<T>, T>;

    static constexpr cs::csp::Id kId{ 0xcbe00219, 0x483c, 0x4f3d, 0xbe2c, 0x9878c3541d7c };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    Diamond& operator=(const Diamond<>& rhs)
    {
        if (this == &rhs)
            return *this;

        m_d0 = rhs.m_d0;
        m_d1 = rhs.m_d1;
        m_d2 = rhs.m_d2;

        return *this;
    }

    [[nodiscard]] bool operator==(const Diamond& rhs) const noexcept
    {
        return m_d0 == rhs.m_d0 && m_d1 == rhs.m_d1 && m_d2 == rhs.m_d2;
    }

    friend cs::csp::processing::BodyProcessor;
};

template<typename T = cs::Dummy>
class SpecialProcessingType : public cs::csp::ISerializable<cs::GetCrtpMainType<SpecialProcessingType<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SpecialProcessingType<>, T>;

    static constexpr cs::csp::Id kId{ 0xada5ded5, 0x2568, 0x44ee, 0x961e, 0xf83491911449 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    SpecialProcessingType& operator=(const SpecialProcessingType<>& rhs)
    {
        if (this == &rhs)
            return *this;

        m_vec = rhs.m_vec;
        m_saaToNS = rhs.m_saaToNS;
        m_saNS = rhs.m_saNS;
        m_pVec = rhs.m_pVec;
        m_pInt = rhs.m_pInt;
        m_ppInt = rhs.m_ppInt;
        m_nullptrInt = rhs.m_nullptrInt;

        return *this;
    }

    [[nodiscard]] bool operator==(const SpecialProcessingType& rhs) const noexcept
    {
        return m_vec == rhs.m_vec
            && m_saaToNS == rhs.m_saaToNS
            && m_saNS == rhs.m_saNS
            && *m_pVec == *rhs.m_pVec   // this claass only for testing proposes, so there is no redundant nullptr checks
            && **m_ppInt == **m_ppInt;
    }

    cs::Vector<Diamond<>> m_vec;
    not_part_of_interfaces::SimplyAssignableAlignedToOne m_saaToNS;
    not_part_of_interfaces::SimplyAssignable m_saNS;
    cs::Vector<Diamond<>>* m_pVec{ nullptr };
    const int* m_pInt{ nullptr };
    const int** m_ppInt{ nullptr };
    const int* m_nullptrInt{ nullptr };

    friend cs::csp::processing::BodyProcessor;
};

#pragma pack(push, 1)

template<typename>
class SimplyAssignableAlignedToOneSimilarType2;

template<typename T = cs::Dummy>
class SimplyAssignableAlignedToOneSimilarType1 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableAlignedToOneSimilarType1<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableAlignedToOneSimilarType1<>, T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0xc009d078, 0xbd81, 0x41ae, 0xb303, 0xb6a361922373 }; // id is same as in SimplyAssignableAlignedToOneSimilarType2 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    [[nodiscard]] bool operator==(const SimplyAssignableAlignedToOneSimilarType1& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == (rhs.m_k & 0xffff);
    }

    SimplyAssignableAlignedToOneSimilarType1<T>& operator=(const SimplyAssignableAlignedToOneSimilarType2<T>& rhs) noexcept;

    char m_j{ 0 };
    int m_k{ 0 };

    friend cs::csp::processing::BodyProcessor;
};

template<typename T = cs::Dummy>
class SimplyAssignableAlignedToOneSimilarType2 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableAlignedToOneSimilarType2<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableAlignedToOneSimilarType2<>, T>;
    using simply_assignable_aligned_to_one_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0xc009d078, 0xbd81, 0x41ae, 0xb303, 0xb6a361922373 }; // hash is same as in AlignedToOneSimilarType1Serializable (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    [[nodiscard]] bool operator==(const SimplyAssignableAlignedToOneSimilarType2& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == rhs.m_k;
    }

    SimplyAssignableAlignedToOneSimilarType2<T>& operator=(const SimplyAssignableAlignedToOneSimilarType1<T>& rhs) noexcept
    {
        m_j = rhs.m_j;
        m_k = static_cast<short>(rhs.m_k);

        return *this;
    }

    char m_j{ 0 };
    short m_k{ 0 };

    friend cs::csp::processing::BodyProcessor;
};

template<typename T>
SimplyAssignableAlignedToOneSimilarType1<T>& SimplyAssignableAlignedToOneSimilarType1<T>::operator=(const SimplyAssignableAlignedToOneSimilarType2<T>& rhs) noexcept
{
    m_j = rhs.m_j;
    m_k = static_cast<int>(rhs.m_k);

    return *this;
}

#pragma pack(pop)

template<typename>
class SimplyAssignableSimilarType2;

template<typename T = cs::Dummy>
class SimplyAssignableSimilarType1 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableSimilarType1<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableSimilarType1<>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0xab625590, 0xcd29, 0x43d0, 0xadfd, 0xc59273696583 }; // id is same as in SimplyAssignableSimilarType2 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    [[nodiscard]] bool operator==(const SimplyAssignableSimilarType1& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == (rhs.m_k & 0xffff);
    }

    SimplyAssignableSimilarType1<T>& operator=(const SimplyAssignableSimilarType2<T>& rhs) noexcept;

    char m_j{ 0 };
    int m_k{ 0 };

    friend cs::csp::processing::BodyProcessor;
};

template<typename T = cs::Dummy>
class SimplyAssignableSimilarType2 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableSimilarType2<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableSimilarType2<>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0xab625590, 0xcd29, 0x43d0, 0xadfd, 0xc59273696583 }; // id is same as in SimplyAssignableSimilarType1 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    [[nodiscard]] bool operator==(const SimplyAssignableSimilarType2& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == rhs.m_k;
    }

    SimplyAssignableSimilarType2<T>& operator=(const SimplyAssignableSimilarType1<T>& rhs) noexcept
    {
        m_j = rhs.m_j;
        m_k = static_cast<short>(rhs.m_k);

        return *this;
    }

    char m_j{ 0 };
    short m_k{ 0 };

    friend cs::csp::processing::BodyProcessor;
};

template<typename T>
SimplyAssignableSimilarType1<T>& SimplyAssignableSimilarType1<T>::operator=(const SimplyAssignableSimilarType2<T>& rhs) noexcept
{
    m_j = rhs.m_j;
    m_k = static_cast<int>(rhs.m_k);

    return *this;
}

template<typename>
class SimilarType2;

template<typename T = cs::Dummy>
class SimilarType1 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimilarType1<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimilarType1<>, T>;

    static constexpr cs::csp::Id kId{ 0x0bcf5909, 0xad8a, 0x48f2, 0xb1fd, 0xe9b781ce0b2d }; // hash is same as in SimilarType2 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    [[nodiscard]] bool operator==(const SimilarType1& rhs) const noexcept
    {
        for (size_t i = 0; i < std::size(m_arrL); ++i)
            if (m_arrL[i] != rhs.m_arrL[i])
                return false;

        for (size_t i = 0; i < std::size(m_sasTs); ++i)
            if (m_sasTs[i] != rhs.m_sasTs[i])
                return false;

        for (size_t i = 0; i < std::size(m_saaToSts); ++i)
            if (m_saaToSts[i] != rhs.m_saaToSts[i])
                return false;

        return m_j == rhs.m_j && m_k == (rhs.m_k & 0xffff);
    }

    SimilarType1<T>& operator=(const SimilarType2<T>& rhs) noexcept;
    

    char m_j{ 0 };
    int m_k{ 0 };
    uint8_t m_arrL[5]{ 0 };
    SimplyAssignableSimilarType1<> m_sasTs[3];
    SimplyAssignableAlignedToOneSimilarType1<> m_saaToSts[3];

    friend cs::csp::processing::BodyProcessor;
};

template<typename T = cs::Dummy>
class SimilarType2 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimilarType2<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimilarType2<>, T>;

    static constexpr cs::csp::Id kId{ 0x0bcf5909, 0xad8a, 0x48f2, 0xb1fd, 0xe9b781ce0b2d }; // id is same as in SimilarType1 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    [[nodiscard]] bool operator==(const SimilarType2& rhs) const noexcept
    {
        for (size_t i = 0; i < std::size(m_arrL); ++i)
            if (m_arrL[i] != rhs.m_arrL[i])
                return false;

        for (size_t i = 0; i < std::size(m_sasTs); ++i)
            if (m_sasTs[i] != rhs.m_sasTs[i])
                return false;

        for (size_t i = 0; i < std::size(m_saaToSts); ++i)
            if (m_saaToSts[i] != rhs.m_saaToSts[i])
                return false;

        return m_j == rhs.m_j && m_k == rhs.m_k;
    }

    SimilarType2<T>& operator=(const SimilarType1<T>& rhs) noexcept
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

    friend cs::csp::processing::BodyProcessor;
};

template<typename T>
SimilarType1<T>& SimilarType1<T>::operator=(const SimilarType2<T>& rhs) noexcept
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

template<typename T = cs::Dummy>
class ManyPointersType : public cs::csp::ISerializable<cs::GetCrtpMainType<ManyPointersType<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<ManyPointersType<>, T>;

    static constexpr cs::csp::Id kId{ 0xc3d3dfdf, 0x27a2, 0x47e2, 0xbddd, 0x671c180db011 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    ManyPointersType& operator=(const ManyPointersType<>& rhs)
    {
        if (this == &rhs)
            return *this;

        m_vec = rhs.m_vec;
        m_vecRecursive = rhs.m_vecRecursive;
        m_pVec = rhs.m_pVec;
        m_rtSpec1 = rhs.m_rtSpec1;
        m_rtSpec2 = rhs.m_rtSpec2;
        
        m_pInt = rhs.m_pInt;
        memcpy(m_intArr, rhs.m_intArr, sizeof(m_intArr));
        m_ppInt = rhs.m_ppInt;
        m_nullptrInt = rhs.m_nullptrInt;

        return *this;
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
            && *m_rtSpec1.pAny->pI == *rhs.m_rtSpec2.pI
            && *m_rtSpec1.pAny->pAny->pAny->pI == *rhs.m_rtSpec2.pI
            && *m_rtSpec1.pAny->pNext->pI == *rhs.m_rtSpec2.pI

            && *m_rtSpec2.pI == *rhs.m_rtSpec2.pI
            && *m_rtSpec2.pAny->pAny->pI == *rhs.m_rtSpec2.pI
            && *m_rtSpec2.pNext->pI == *rhs.m_rtSpec2.pI

            && *m_pInt == *rhs.m_pInt
            && memcmp(m_intArr, rhs.m_intArr, sizeof(m_intArr)) == 0
            && **m_ppInt == **rhs.m_ppInt
            && m_nullptrInt == nullptr;
    }

    cs::Vector<int*, cs::RawStrategicAllocatorHelper<int*>> m_vec;
    cs::Vector<cs::Vector<int*, cs::RawStrategicAllocatorHelper<int*>>*, cs::RawStrategicAllocatorHelper<cs::Vector<int*, cs::RawStrategicAllocatorHelper<int*>>*>> m_vecRecursive;
    cs::Vector<int*, cs::RawStrategicAllocatorHelper<int*>>* m_pVec;

    not_part_of_interfaces::RecursiveTestSpecial1 m_rtSpec1;
    not_part_of_interfaces::RecursiveTestSpecial2 m_rtSpec2;

    int* m_pInt{ nullptr };
    int m_intArr[3]{ 0 };
    int** m_ppInt{ nullptr };
    int* m_nullptrInt{ nullptr };

    friend cs::csp::processing::BodyProcessor;
};

template<typename> 
class SForAllModesTests_Version2;

// DForAllModesTests shall be used in deserialization of SForAllModesTests, but only with sizeOfIntegersMayBeNotEqual flag set
template<typename T = cs::Dummy>
class DForAllModesTests : public cs::csp::ISerializable<cs::GetCrtpMainType<DForAllModesTests<>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<DForAllModesTests<>, T>;

    static constexpr cs::csp::Id kId{ 0x953e9f24, 0xc725, 0x4903, 0x8219, 0xf37d084ef557 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 3, 2, 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    template<typename T2>
    cs::Status init(const SForAllModesTests_Version2<T2>& rhs);

    [[nodiscard]] bool operator==(const DForAllModesTests<>& rhs) const noexcept
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

    SimplyAssignableDescendant<> m_saDs;
    Diamond<> m_diamond;
    SpecialProcessingType<> m_sptCs;
    SimplyAssignableAlignedToOneSimilarType2<> m_saaToStS;
    SimplyAssignableSimilarType2<> m_saStS;
    SimilarType2<> m_stS;
    ManyPointersType<> m_mpt;

    friend cs::csp::processing::BodyProcessor;
    friend SForAllModesTests_Version2<T>;
};

template<typename T = cs::Dummy>
class SimplyAssignableWithoutSerializationFunctions : public cs::csp::ISerializable<cs::GetCrtpMainType<SimplyAssignableWithoutSerializationFunctions<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimplyAssignableWithoutSerializationFunctions<>, T>;
    using simply_assignable_tag = std::true_type;

    static constexpr cs::csp::Id kId{ 0x4e613dd4, 0xf408, 0x4f5b, 0xa18f, 0xec9e8f4c86ce };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    uint32_t m_i{ 0 };

    [[nodiscard]] bool operator==(const SimplyAssignableWithoutSerializationFunctions& rhs) const
    {
        return m_i == rhs.m_i;
    }

    friend cs::csp::processing::BodyProcessor;
};

template<typename T = cs::Dummy>
class ContainSimplyAssignableWithoutSerializationFunctions : public cs::csp::ISerializable<cs::GetCrtpMainType<ContainSimplyAssignableWithoutSerializationFunctions<>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<ContainSimplyAssignableWithoutSerializationFunctions<>, T>;

    static constexpr cs::csp::Id kId{ 0xf2b9a015, 0x1ec1, 0x4b06, 0xa313, 0x158b97af8fa4 };
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kPrivateVersions[] = { 0 };
    static consteval const cs::csp::Interface& getInterface() noexcept { return properties; }

    SimplyAssignableWithoutSerializationFunctions<> m_sawsf;
    not_part_of_interfaces::SimplyAssignableWithoutSerializationFunctions m_npfSawsf;

    [[nodiscard]] bool operator==(const ContainSimplyAssignableWithoutSerializationFunctions& rhs) const
    {
        return m_sawsf == rhs.m_sawsf && m_npfSawsf == rhs.m_npfSawsf;
    }

    friend cs::csp::processing::BodyProcessor;
};

} // namespace interface_for_test
