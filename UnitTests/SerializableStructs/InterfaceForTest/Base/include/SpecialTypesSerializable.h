/**
 * @file SpecialTypesSerializable.h
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

struct EmptyTypeNotSerializable
{
    using empty_type_tag = std::true_type;
};

template<typename T = Dummy>
class EmptyTypeSerializable : public csp::ISerializable<GetCrtpMainType<EmptyTypeSerializable<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<EmptyTypeSerializable<T>, T>;
    using empty_type_tag = std::true_type;

    static constexpr Uuid kUuid = helpers::getUuid(0x7f17247f, 0x094b, 0x4cda, 0x9968, 0x66e31b88063c);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };
};

#pragma pack(push, 1)

struct SimpleAssignableAlignedToOneNotSerializable
{
    using simple_assignable_tag = std::true_type;

    uint16_t a{ 0 };
    uint8_t s{ 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneNotSerializable& rhs) const noexcept
    {
        return a == rhs.a && s == rhs.s;
    }
};

template<typename>
class SimpleAssignableAlignedToOneSerializable_Version1;

template<typename T = Dummy>
class SimpleAssignableAlignedToOneSerializable : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSerializable<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSerializable<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kUuid = helpers::getUuid(0xf2d69dcd, 0x4e24, 0x4c65, 0x9f76, 0xd517be1daccd);
    static constexpr csp::interface_version_t kInterfaceVersion = 3;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 3, 1, 0 };

    SimpleAssignableAlignedToOneSerializable() { }
    template<typename T2>
    Status init(const SimpleAssignableAlignedToOneSerializable_Version1<T2>& rhs);

    SimpleAssignableAlignedToOneSerializable& operator=(const SimpleAssignableAlignedToOneSerializable& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        m_x = rhs.m_x, m_y = rhs.m_y;

        return *this;
    }

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSerializable& rhs) const noexcept
    {
        return m_x == rhs.m_x && m_y == rhs.m_y;
    }

    uint8_t m_x{ 0 };
    uint8_t m_y{ 0 };

    friend csp::processing::DataProcessor;
    friend SimpleAssignableAlignedToOneSerializable_Version1;
};

#pragma pack(pop)

struct SimpleAssignableNotSerializable
{
    using simple_assignable_tag = std::true_type;

    uint16_t q{ 0 };
    uint64_t w{ 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableNotSerializable& rhs) const noexcept
    {
        return q == rhs.q && w == rhs.w;
    }
};

template<typename>
class SimpleAssignableSerializable_Version0;

template<typename T = Dummy>
class SimpleAssignableSerializable : public csp::ISerializable<GetCrtpMainType<SimpleAssignableSerializable<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableSerializable<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kUuid = helpers::getUuid(0x81582aae, 0x4a33, 0x4181, 0x8dda, 0xed092c23bccc);
    static constexpr csp::interface_version_t kInterfaceVersion = 3;         // latest version among all dependable structs
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 2, 0 };

    template<typename T2>
    Status init(const SimpleAssignableSerializable_Version0<T2>& rhs);

    SimpleAssignableSerializable& operator=(const SimpleAssignableSerializable<>& rhs) noexcept
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

    [[nodiscard]] bool operator==(const SimpleAssignableSerializable& rhs)                  const noexcept
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
    EmptyTypeSerializable<> m_et;
    EmptyTypeNotSerializable m_et2;
    SimpleAssignableAlignedToOneSerializable<> m_saaToS;
    SimpleAssignableAlignedToOneNotSerializable m_saaToNS;
    SimpleAssignableNotSerializable m_saNS;

    uint32_t m_arrI32[3]{ 0 };
    EmptyTypeSerializable<> m_arrEtS[3]{};
    EmptyTypeNotSerializable m_arrEtNS[3]{};
    SimpleAssignableAlignedToOneSerializable<> m_arrSaaTos[3]{};
    SimpleAssignableAlignedToOneNotSerializable m_arrSaaToNS[3]{};
    SimpleAssignableNotSerializable m_arrSaNS[3]{};

    uint64_t m_vx{ 0 };

    friend csp::processing::DataProcessor;
    friend SimpleAssignableSerializable_Version0;
};

template<typename>
class SimpleAssignableDescendantSerializable_Version0;

template<typename T = Dummy>
struct SimpleAssignableDescendantSerializable : public SimpleAssignableSerializable<GetCrtpMainType<SimpleAssignableDescendantSerializable<T>, T>>
{
    using instance_type = GetCrtpMainType<SimpleAssignableDescendantSerializable<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kUuid = helpers::getUuid(0x81d67474, 0xce11, 0x465e, 0x8558, 0xe10e84939b84);
    static constexpr csp::interface_version_t kInterfaceVersion = 3;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 2, 0 };

    uint32_t m_d{ 0 };

    template<typename T2>
    Status init(const SimpleAssignableDescendantSerializable_Version0<T2>& rhs);

    SimpleAssignableDescendantSerializable& operator=(const SimpleAssignableDescendantSerializable<>& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        SimpleAssignableSerializable<>::operator=(rhs);
        m_d = rhs.m_d;

        return *this;
    }
    
    [[nodiscard]] operator SimpleAssignableSerializable<>& () noexcept
    {
        return  *static_cast<SimpleAssignableSerializable<>*>(
                    static_cast<void*>(
                        static_cast<SimpleAssignableSerializable<instance_type>*>(this)));
    }

    [[nodiscard]] operator const SimpleAssignableSerializable<>& () const noexcept
    {
        return  *static_cast<const SimpleAssignableSerializable<>*>(
                    static_cast<const void*>(
                        static_cast<const SimpleAssignableSerializable<instance_type>*>(this)));
    }
    
    [[nodiscard]] bool operator==(const SimpleAssignableDescendantSerializable& rhs) const noexcept
    {
        return m_d == rhs.m_d && SimpleAssignableSerializable<instance_type>::operator==(rhs);
    }

    friend csp::processing::DataProcessor;
    friend SimpleAssignableDescendantSerializable_Version0;
};

class DynamicPolymorphicNotSerializable
{
public:
    virtual ~DynamicPolymorphicNotSerializable() {}

    static constexpr Uuid kUuid = helpers::getUuid(0xad460dc2, 0x429c, 0x4313, 0xa7c5, 0x06fca38389a0);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const DynamicPolymorphicNotSerializable& rhs) const noexcept
    {
        return m_r == rhs.m_r && memcmp(m_arrR, rhs.m_arrR, sizeof(m_arrR)) == 0;
    }

    uint8_t m_r{ 0 };
    uint8_t m_arrR[3]{ 0 };

    friend csp::processing::DataProcessor;
};

template<typename T = Dummy>
class DynamicPolymorphicSerializable : public csp::ISerializable<GetCrtpMainType<DynamicPolymorphicSerializable<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<DynamicPolymorphicSerializable<T>, T>;

    virtual ~DynamicPolymorphicSerializable() {}

    static constexpr Uuid kUuid = helpers::getUuid(0x7df21aa3, 0x9999, 0x4fa7, 0xa34d, 0xd7190e818392);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const DynamicPolymorphicSerializable& rhs) const noexcept
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
    DynamicPolymorphicNotSerializable m_dpNS{};

    uint8_t m_arrO[3]{ 0 };
    DynamicPolymorphicNotSerializable m_arrDpNS[3]{};

    friend csp::processing::DataProcessor;
};

struct DiamondBaseNotSerializable
{
    [[nodiscard]] bool operator==(const DiamondBaseNotSerializable& rhs) const noexcept
    {
        return m_d0 == rhs.m_d0;
    }

    uint32_t m_d0{ 0 };
};

struct DiamondEdge1NotSerializable : virtual public DiamondBaseNotSerializable
{
    [[nodiscard]] bool operator==(const DiamondEdge1NotSerializable& rhs) const noexcept
    {
        return m_d1 == rhs.m_d1;
    }

    uint32_t m_d1{ 0 };

    friend csp::processing::DataProcessor;
};

struct DiamondEdge2NotSerializable : virtual public DiamondBaseNotSerializable
{
    [[nodiscard]] bool operator==(const DiamondEdge2NotSerializable& rhs) const noexcept
    {
        return m_d2 == rhs.m_d2;
    }

    uint32_t m_d2{ 0 };

    friend csp::processing::DataProcessor;
};

template<typename T = Dummy>
class DiamondSerializable 
    : public csp::ISerializable<GetCrtpMainType<DiamondSerializable<T>, T >>
    , public DiamondEdge1NotSerializable
    , public DiamondEdge2NotSerializable
{
public:
    using instance_type = GetCrtpMainType<DiamondSerializable<T>, T>;

    static constexpr Uuid kUuid = helpers::getUuid(0xcbe00219, 0x483c, 0x4f3d, 0xbe2c, 0x9878c3541d7c);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    DiamondSerializable& operator=(const DiamondSerializable<>& rhs)
    {
        if (this == &rhs)
            return *this;

        m_d0 = rhs.m_d0;
        m_d1 = rhs.m_d1;
        m_d2 = rhs.m_d2;

        return *this;
    }

    [[nodiscard]] bool operator==(const DiamondSerializable& rhs) const noexcept
    {
        return m_d0 == rhs.m_d0 && m_d1 == rhs.m_d1 && m_d2 == rhs.m_d2;
    }

    friend csp::processing::DataProcessor;
};

template<typename T = Dummy>
class SpecialProcessingTypeContainSerializable : public csp::ISerializable<GetCrtpMainType<SpecialProcessingTypeContainSerializable<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SpecialProcessingTypeContainSerializable<T>, T>;

    static constexpr Uuid kUuid = helpers::getUuid(0xada5ded5, 0x2568, 0x44ee, 0x961e, 0xf83491911449);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    SpecialProcessingTypeContainSerializable& operator=(const SpecialProcessingTypeContainSerializable<>& rhs)
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

    [[nodiscard]] bool operator==(const SpecialProcessingTypeContainSerializable& rhs) const noexcept
    {
        return m_vec == rhs.m_vec
            && m_saaToNS == rhs.m_saaToNS
            && m_saNS == rhs.m_saNS
            && *m_pVec == *rhs.m_pVec   // this claass only for testing proposes, so there is no redundant nullptr checks
            && **m_ppInt == **m_ppInt;
    }

    Vector<DiamondSerializable<>> m_vec;
    SimpleAssignableAlignedToOneNotSerializable m_saaToNS;
    SimpleAssignableNotSerializable m_saNS;
    Vector<DiamondSerializable<>>* m_pVec{ nullptr };
    const int* m_pInt{ nullptr };
    const int** m_ppInt{ nullptr };
    const int* m_nullptrInt{ nullptr };

    friend csp::processing::DataProcessor;
};

#pragma pack(push, 1)

template<typename>
class SimpleAssignableAlignedToOneSimilarType2Serializable;

template<typename T = Dummy>
class SimpleAssignableAlignedToOneSimilarType1Serializable : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSimilarType1Serializable<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSimilarType1Serializable<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kUuid = helpers::getUuid(0xc009d078, 0xbd81, 0x41ae, 0xb303, 0xb6a361922373); // uuid is same as in SimpleAssignableAlignedToOneSimilarType2Serializable (need for tests)
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSimilarType1Serializable& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == (rhs.m_k & 0xffff);
    }

    SimpleAssignableAlignedToOneSimilarType1Serializable<T>& operator=(const SimpleAssignableAlignedToOneSimilarType2Serializable<T>& rhs) noexcept;

    char m_j{ 0 };
    int m_k{ 0 };

    friend csp::processing::DataProcessor;
};

template<typename T = Dummy>
class SimpleAssignableAlignedToOneSimilarType2Serializable : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSimilarType2Serializable<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSimilarType2Serializable<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kUuid = helpers::getUuid(0xc009d078, 0xbd81, 0x41ae, 0xb303, 0xb6a361922373); // hash is same as in AlignedToOneSimilarType1Serializable (need for tests)
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSimilarType2Serializable& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == rhs.m_k;
    }

    SimpleAssignableAlignedToOneSimilarType2Serializable<T>& operator=(const SimpleAssignableAlignedToOneSimilarType1Serializable<T>& rhs) noexcept
    {
        m_j = rhs.m_j;
        m_k = static_cast<short>(rhs.m_k);

        return *this;
    }

    char m_j{ 0 };
    short m_k{ 0 };

    friend csp::processing::DataProcessor;
};

template<typename T>
SimpleAssignableAlignedToOneSimilarType1Serializable<T>& SimpleAssignableAlignedToOneSimilarType1Serializable<T>::operator=(const SimpleAssignableAlignedToOneSimilarType2Serializable<T>& rhs) noexcept
{
    m_j = rhs.m_j;
    m_k = static_cast<int>(rhs.m_k);

    return *this;
}

#pragma pack(pop)

template<typename>
class SimpleAssignableSimilarType2Serializable;

template<typename T = Dummy>
class SimpleAssignableSimilarType1Serializable : public csp::ISerializable<GetCrtpMainType<SimpleAssignableSimilarType1Serializable<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableSimilarType1Serializable<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kUuid = helpers::getUuid(0xab625590, 0xcd29, 0x43d0, 0xadfd, 0xc59273696583); // uuid is same as in SimpleAssignableSimilarType2Serializable (need for tests)
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableSimilarType1Serializable& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == (rhs.m_k & 0xffff);
    }

    SimpleAssignableSimilarType1Serializable<T>& operator=(const SimpleAssignableSimilarType2Serializable<T>& rhs) noexcept;

    char m_j{ 0 };
    int m_k{ 0 };

    friend csp::processing::DataProcessor;
};

template<typename T = Dummy>
class SimpleAssignableSimilarType2Serializable : public csp::ISerializable<GetCrtpMainType<SimpleAssignableSimilarType2Serializable<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableSimilarType2Serializable<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr Uuid kUuid = helpers::getUuid(0xab625590, 0xcd29, 0x43d0, 0xadfd, 0xc59273696583); // uuid is same as in SimpleAssignableSimilarType1Serializable (need for tests)
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableSimilarType2Serializable& rhs) const noexcept
    {
        return m_j == rhs.m_j && m_k == rhs.m_k;
    }

    SimpleAssignableSimilarType2Serializable<T>& operator=(const SimpleAssignableSimilarType1Serializable<T>& rhs) noexcept
    {
        m_j = rhs.m_j;
        m_k = static_cast<short>(rhs.m_k);

        return *this;
    }

    char m_j{ 0 };
    short m_k{ 0 };

    friend csp::processing::DataProcessor;
};

template<typename T>
SimpleAssignableSimilarType1Serializable<T>& SimpleAssignableSimilarType1Serializable<T>::operator=(const SimpleAssignableSimilarType2Serializable<T>& rhs) noexcept
{
    m_j = rhs.m_j;
    m_k = static_cast<int>(rhs.m_k);

    return *this;
}

template<typename>
class SimilarType2Serializable;

template<typename T = Dummy>
class SimilarType1Serializable : public csp::ISerializable<GetCrtpMainType<SimilarType1Serializable<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SimilarType1Serializable<T>, T>;

    static constexpr Uuid kUuid = helpers::getUuid(0x0bcf5909, 0xad8a, 0x48f2, 0xb1fd, 0xe9b781ce0b2d); // hash is same as in SimilarType2Serializable (need for tests)
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimilarType1Serializable& rhs) const noexcept
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

    SimilarType1Serializable<T>& operator=(const SimilarType2Serializable<T>& rhs) noexcept;
    

    char m_j{ 0 };
    int m_k{ 0 };
    uint8_t m_arrL[5]{ 0 };
    SimpleAssignableSimilarType1Serializable<> m_sasTs[3];
    SimpleAssignableAlignedToOneSimilarType1Serializable<> m_saaToSts[3];

    friend csp::processing::DataProcessor;
};

template<typename T = Dummy>
class SimilarType2Serializable : public csp::ISerializable<GetCrtpMainType<SimilarType2Serializable<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SimilarType2Serializable<T>, T>;

    static constexpr Uuid kUuid = helpers::getUuid(0x0bcf5909, 0xad8a, 0x48f2, 0xb1fd, 0xe9b781ce0b2d); // uuid is same as in SimilarType1Serializable (need for tests)
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const SimilarType2Serializable& rhs) const noexcept
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

    SimilarType2Serializable<T>& operator=(const SimilarType1Serializable<T>& rhs) noexcept
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
    SimpleAssignableSimilarType2Serializable<> m_sasTs[3];
    SimpleAssignableAlignedToOneSimilarType2Serializable<> m_saaToSts[3];

    friend csp::processing::DataProcessor;
};

template<typename T>
SimilarType1Serializable<T>& SimilarType1Serializable<T>::operator=(const SimilarType2Serializable<T>& rhs) noexcept
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


template<typename T = Dummy>
class ManyPointersTypeSerializable : public csp::ISerializable<GetCrtpMainType<ManyPointersTypeSerializable<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<ManyPointersTypeSerializable<T>, T>;

    static constexpr Uuid kUuid = helpers::getUuid(0xc3d3dfdf, 0x27a2, 0x47e2, 0xbddd, 0x671c180db011);
    static constexpr csp::interface_version_t kInterfaceVersion = 0;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 0 };

    ManyPointersTypeSerializable& operator=(const ManyPointersTypeSerializable<>& rhs)
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

    [[nodiscard]] bool operator==(const ManyPointersTypeSerializable& rhs) const noexcept
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

    Vector<int*, RawStrategicAllocatorHelper<int*>> m_vec;
    Vector<Vector<int*, RawStrategicAllocatorHelper<int*>>*, RawStrategicAllocatorHelper<Vector<int*, RawStrategicAllocatorHelper<int*>>*>> m_vecRecursive;
    Vector<int*, RawStrategicAllocatorHelper<int*>>* m_pVec;

    RecursiveTestSpecial1 m_rtSpec1;
    RecursiveTestSpecial2 m_rtSpec2;

    int* m_pInt{ nullptr };
    int m_intArr[3]{ 0 };
    int** m_ppInt{ nullptr };
    int* m_nullptrInt{ nullptr };

    friend csp::processing::DataProcessor;
};

template<typename> 
class SForAllModesTests_Version2;

// DForAllModesTests shall be used in deserialization of SForAllModesTests, but only with sizeOfArithmeticTypesMayBeNotEqual flag set
template<typename T = Dummy>
class DForAllModesTests : public csp::ISerializable<GetCrtpMainType<DForAllModesTests<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<DForAllModesTests<T>, T>;

    static constexpr Uuid kUuid = helpers::getUuid(0x953e9f24, 0xc725, 0x4903, 0x8219, 0xf37d084ef557);
    static constexpr csp::interface_version_t kInterfaceVersion = 3;
    static constexpr csp::interface_version_t kVersionsHierarchy[] = { 3, 2, 0 };

    template<typename T2>
    Status init(const SForAllModesTests_Version2<T2>& rhs);

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

    SimpleAssignableDescendantSerializable<> m_saDs;
    DiamondSerializable<> m_diamond;
    SpecialProcessingTypeContainSerializable<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType2Serializable<> m_saaToStS;
    SimpleAssignableSimilarType2Serializable<> m_saStS;
    SimilarType2Serializable<> m_stS;
    ManyPointersTypeSerializable<> m_mpt;

    friend csp::processing::DataProcessor;
    friend SForAllModesTests_Version2;
};

} // namespace special_types
