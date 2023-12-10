/**
 * @file SerializableStructs/interface_for_test/include/interface_for_test/SpecialTypesSerializable.h
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
class EmptyType : public cs::csp::ISerializable<cs::GetCrtpMainType<EmptyType<T>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<EmptyType<T>, T>;
    using empty_type_tag = std::true_type;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0x7f17247f, 0x094b, 0x4cda, 0x9968, 0x66e31b88063c);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };
};

#pragma pack(push, 1)

template<typename>
class SimpleAssignableAlignedToOne_Version1;

template<typename T = cs::Dummy>
class SimpleAssignableAlignedToOne : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleAssignableAlignedToOne<T>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleAssignableAlignedToOne<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0xf2d69dcd, 0x4e24, 0x4c65, 0x9f76, 0xd517be1daccd);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 3, 1, 0 };

    SimpleAssignableAlignedToOne() { }
    template<typename T2>
    cs::Status init(const SimpleAssignableAlignedToOne_Version1<T2>& rhs);

    SimpleAssignableAlignedToOne& operator=(const SimpleAssignableAlignedToOne& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        m_x = rhs.m_x, m_y = rhs.m_y;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOne& rhs) const noexcept
    {
        return m_x == rhs.m_x && m_y == rhs.m_y;
    }

    uint8_t m_x{ 0 };
    uint8_t m_y{ 0 };

    friend cs::csp::processing::DataProcessor;
    friend SimpleAssignableAlignedToOne_Version1;
};

#pragma pack(pop)

template<typename>
class SimpleAssignable_Version0;

template<typename T = cs::Dummy>
class SimpleAssignable : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleAssignable<T>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleAssignable<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0x81582aae, 0x4a33, 0x4181, 0x8dda, 0xed092c23bccc);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 3;         // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 2, 0 };

    template<typename T2>
    cs::Status init(const SimpleAssignable_Version0<T2>& rhs);

    SimpleAssignable& operator=(const SimpleAssignable<>& rhs) noexcept
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

    [[nodiscard]] bool operator==(const SimpleAssignable& rhs)                  const noexcept
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
    SimpleAssignableAlignedToOne<> m_saaToS;
    not_part_of_interfaces::SimpleAssignableAlignedToOne m_saaToNS;
    not_part_of_interfaces::SimpleAssignable m_saNS;

    uint32_t m_arrI32[3]{ 0 };
    EmptyType<> m_arrEtS[3]{};
    not_part_of_interfaces::EmptyType m_arrEtNS[3]{};
    SimpleAssignableAlignedToOne<> m_arrSaaTos[3]{};
    not_part_of_interfaces::SimpleAssignableAlignedToOne m_arrSaaToNS[3]{};
    not_part_of_interfaces::SimpleAssignable m_arrSaNS[3]{};

    uint64_t m_vx{ 0 };

    friend cs::csp::processing::DataProcessor;
    friend SimpleAssignable_Version0;
};

template<typename>
class SimpleAssignableDescendant_Version0;

template<typename T = cs::Dummy>
struct SimpleAssignableDescendant : public SimpleAssignable<cs::GetCrtpMainType<SimpleAssignableDescendant<T>, T>>
{
    using instance_type = cs::GetCrtpMainType<SimpleAssignableDescendant<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0x81d67474, 0xce11, 0x465e, 0x8558, 0xe10e84939b84);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 2, 0 };

    uint32_t m_d{ 0 };

    template<typename T2>
    cs::Status init(const SimpleAssignableDescendant_Version0<T2>& rhs);

    SimpleAssignableDescendant& operator=(const SimpleAssignableDescendant<>& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        SimpleAssignable<>::operator=(rhs);
        m_d = rhs.m_d;

        return *this;
    }
    
    [[nodiscard]] operator SimpleAssignable<>& () noexcept
    {
        return  *static_cast<SimpleAssignable<>*>(
                    static_cast<void*>(
                        static_cast<SimpleAssignable<instance_type>*>(this)));
    }

    [[nodiscard]] operator const SimpleAssignable<>& () const noexcept
    {
        return  *static_cast<const SimpleAssignable<>*>(
                    static_cast<const void*>(
                        static_cast<const SimpleAssignable<instance_type>*>(this)));
    }
    
    [[nodiscard]] bool operator==(const SimpleAssignableDescendant& rhs) const noexcept
    {
        return m_d == rhs.m_d && SimpleAssignable<instance_type>::operator==(rhs);
    }

    friend cs::csp::processing::DataProcessor;
    friend SimpleAssignableDescendant_Version0;
};

template<typename T = cs::Dummy>
class DynamicPolymorphic : public cs::csp::ISerializable<cs::GetCrtpMainType<DynamicPolymorphic<T>, T>>
{
public:
    using instance_type = cs::GetCrtpMainType<DynamicPolymorphic<T>, T>;

    virtual ~DynamicPolymorphic() {}

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0x7df21aa3, 0x9999, 0x4fa7, 0xa34d, 0xd7190e818392);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

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

    friend cs::csp::processing::DataProcessor;
};

template<typename T = cs::Dummy>
class Diamond 
    : public cs::csp::ISerializable<cs::GetCrtpMainType<Diamond<T>, T >>
    , public not_part_of_interfaces::DiamondEdge1
    , public not_part_of_interfaces::DiamondEdge2
{
public:
    using instance_type = cs::GetCrtpMainType<Diamond<T>, T>;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0xcbe00219, 0x483c, 0x4f3d, 0xbe2c, 0x9878c3541d7c);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

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

    friend cs::csp::processing::DataProcessor;
};

template<typename T = cs::Dummy>
class SpecialProcessingType : public cs::csp::ISerializable<cs::GetCrtpMainType<SpecialProcessingType<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SpecialProcessingType<T>, T>;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0xada5ded5, 0x2568, 0x44ee, 0x961e, 0xf83491911449);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

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
    not_part_of_interfaces::SimpleAssignableAlignedToOne m_saaToNS;
    not_part_of_interfaces::SimpleAssignable m_saNS;
    cs::Vector<Diamond<>>* m_pVec{ nullptr };
    const int* m_pInt{ nullptr };
    const int** m_ppInt{ nullptr };
    const int* m_nullptrInt{ nullptr };

    friend cs::csp::processing::DataProcessor;
};

#pragma pack(push, 1)

template<typename>
class SimpleAssignableAlignedToOneSimilarType2;

template<typename T = cs::Dummy>
class SimpleAssignableAlignedToOneSimilarType1 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleAssignableAlignedToOneSimilarType1<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleAssignableAlignedToOneSimilarType1<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0xc009d078, 0xbd81, 0x41ae, 0xb303, 0xb6a361922373); // id is same as in SimpleAssignableAlignedToOneSimilarType2 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSimilarType1& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == (rhs.m_k & 0xffff);
    }

    SimpleAssignableAlignedToOneSimilarType1<T>& operator=(const SimpleAssignableAlignedToOneSimilarType2<T>& rhs) noexcept;

    char m_j{ 0 };
    int m_k{ 0 };

    friend cs::csp::processing::DataProcessor;
};

template<typename T = cs::Dummy>
class SimpleAssignableAlignedToOneSimilarType2 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleAssignableAlignedToOneSimilarType2<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleAssignableAlignedToOneSimilarType2<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0xc009d078, 0xbd81, 0x41ae, 0xb303, 0xb6a361922373); // hash is same as in AlignedToOneSimilarType1Serializable (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSimilarType2& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == rhs.m_k;
    }

    SimpleAssignableAlignedToOneSimilarType2<T>& operator=(const SimpleAssignableAlignedToOneSimilarType1<T>& rhs) noexcept
    {
        m_j = rhs.m_j;
        m_k = static_cast<short>(rhs.m_k);

        return *this;
    }

    char m_j{ 0 };
    short m_k{ 0 };

    friend cs::csp::processing::DataProcessor;
};

template<typename T>
SimpleAssignableAlignedToOneSimilarType1<T>& SimpleAssignableAlignedToOneSimilarType1<T>::operator=(const SimpleAssignableAlignedToOneSimilarType2<T>& rhs) noexcept
{
    m_j = rhs.m_j;
    m_k = static_cast<int>(rhs.m_k);

    return *this;
}

#pragma pack(pop)

template<typename>
class SimpleAssignableSimilarType2;

template<typename T = cs::Dummy>
class SimpleAssignableSimilarType1 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleAssignableSimilarType1<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleAssignableSimilarType1<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0xab625590, 0xcd29, 0x43d0, 0xadfd, 0xc59273696583); // id is same as in SimpleAssignableSimilarType2 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableSimilarType1& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == (rhs.m_k & 0xffff);
    }

    SimpleAssignableSimilarType1<T>& operator=(const SimpleAssignableSimilarType2<T>& rhs) noexcept;

    char m_j{ 0 };
    int m_k{ 0 };

    friend cs::csp::processing::DataProcessor;
};

template<typename T = cs::Dummy>
class SimpleAssignableSimilarType2 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimpleAssignableSimilarType2<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimpleAssignableSimilarType2<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0xab625590, 0xcd29, 0x43d0, 0xadfd, 0xc59273696583); // id is same as in SimpleAssignableSimilarType1 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableSimilarType2& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == rhs.m_k;
    }

    SimpleAssignableSimilarType2<T>& operator=(const SimpleAssignableSimilarType1<T>& rhs) noexcept
    {
        m_j = rhs.m_j;
        m_k = static_cast<short>(rhs.m_k);

        return *this;
    }

    char m_j{ 0 };
    short m_k{ 0 };

    friend cs::csp::processing::DataProcessor;
};

template<typename T>
SimpleAssignableSimilarType1<T>& SimpleAssignableSimilarType1<T>::operator=(const SimpleAssignableSimilarType2<T>& rhs) noexcept
{
    m_j = rhs.m_j;
    m_k = static_cast<int>(rhs.m_k);

    return *this;
}

template<typename>
class SimilarType2;

template<typename T = cs::Dummy>
class SimilarType1 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimilarType1<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimilarType1<T>, T>;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0x0bcf5909, 0xad8a, 0x48f2, 0xb1fd, 0xe9b781ce0b2d); // hash is same as in SimilarType2 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

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
    SimpleAssignableSimilarType1<> m_sasTs[3];
    SimpleAssignableAlignedToOneSimilarType1<> m_saaToSts[3];

    friend cs::csp::processing::DataProcessor;
};

template<typename T = cs::Dummy>
class SimilarType2 : public cs::csp::ISerializable<cs::GetCrtpMainType<SimilarType2<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<SimilarType2<T>, T>;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0x0bcf5909, 0xad8a, 0x48f2, 0xb1fd, 0xe9b781ce0b2d); // id is same as in SimilarType1 (need for tests)
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

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
    SimpleAssignableSimilarType2<> m_sasTs[3];
    SimpleAssignableAlignedToOneSimilarType2<> m_saaToSts[3];

    friend cs::csp::processing::DataProcessor;
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

struct RecursiveTestSpecial1;

struct RecursiveTestSpecial2
{
    int* pI{ nullptr };
    RecursiveTestSpecial2* pNext{ nullptr };
    RecursiveTestSpecial1* pAny{ nullptr };
};

struct RecursiveTestSpecial1
{
    RecursiveTestSpecial2* pAny{ nullptr };
};


template<typename T = cs::Dummy>
class ManyPointersType : public cs::csp::ISerializable<cs::GetCrtpMainType<ManyPointersType<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<ManyPointersType<T>, T>;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0xc3d3dfdf, 0x27a2, 0x47e2, 0xbddd, 0x671c180db011);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 0;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 0 };

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

    RecursiveTestSpecial1 m_rtSpec1;
    RecursiveTestSpecial2 m_rtSpec2;

    int* m_pInt{ nullptr };
    int m_intArr[3]{ 0 };
    int** m_ppInt{ nullptr };
    int* m_nullptrInt{ nullptr };

    friend cs::csp::processing::DataProcessor;
};

template<typename> 
class SForAllModesTests_Version2;

// DForAllModesTests shall be used in deserialization of SForAllModesTests, but only with sizeOfArithmeticTypesMayBeNotEqual flag set
template<typename T = cs::Dummy>
class DForAllModesTests : public cs::csp::ISerializable<cs::GetCrtpMainType<DForAllModesTests<T>, T >>
{
public:
    using instance_type = cs::GetCrtpMainType<DForAllModesTests<T>, T>;

    static constexpr cs::Uuid kId = cs::helpers::getUuid(0x953e9f24, 0xc725, 0x4903, 0x8219, 0xf37d084ef557);
    static constexpr cs::csp::interface_version_t kInterfaceVersion = 3;
    static constexpr cs::csp::interface_version_t kVersionsHierarchy[] = { 3, 2, 0 };

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

    SimpleAssignableDescendant<> m_saDs;
    Diamond<> m_diamond;
    SpecialProcessingType<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType2<> m_saaToStS;
    SimpleAssignableSimilarType2<> m_saStS;
    SimilarType2<> m_stS;
    ManyPointersType<> m_mpt;

    friend cs::csp::processing::DataProcessor;
    friend SForAllModesTests_Version2;
};

} // namespace interface_for_test
