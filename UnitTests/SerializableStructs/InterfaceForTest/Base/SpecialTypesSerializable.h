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
    using empty_type = std::true_type;
};

template<typename T = Dummy>
class EmptyTypeSerializable : public csp::ISerializable<GetCrtpMainType<EmptyTypeSerializable<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<EmptyTypeSerializable<T>, T>;
    using empty_type = std::true_type;

    static constexpr uint64_t kNameHash = 0;
    static constexpr uint32_t kInterfaceVersion = 0;         // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };
};

#pragma pack(push, 1)

struct SimpleAssignableAlignedToOneNotSerializable
{
    using simple_assignable = std::true_type;

    uint16_t a{ 0 };
    uint8_t s{ 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneNotSerializable& rhs) const noexcept
    {
        return a == rhs.a && s == rhs.s;
    }
};

template<typename T = Dummy>
class SimpleAssignableAlignedToOneSerializable : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSerializable<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSerializable<T>, T>;
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 1;
    static constexpr uint32_t kInterfaceVersion = 2;            // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 2, 1, 0 };

    [[nodiscard]] uint8_t& getX()                 noexcept { return m_x; }    // getters here are only need for testing proposes
    [[nodiscard]] const uint8_t& getX()     const noexcept { return m_x; }    // (not required for serialization itself)
    [[nodiscard]] uint8_t& getY()                 noexcept { return m_y; }
    [[nodiscard]] const uint8_t& getY()     const noexcept { return m_y; }

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSerializable& rhs) const noexcept
    {
        return m_x == rhs.m_x && m_y == rhs.m_y;
    }

private:
    uint8_t m_x{ 0 };
    uint8_t m_y{ 0 };

    friend csp::processing::DataProcessor;
};

#pragma pack(pop)

struct SimpleAssignableNotSerializable
{
    using simple_assignable = std::true_type;

    uint16_t q{ 0 };
    uint64_t w{ 0 };

    [[nodiscard]] bool operator==(const SimpleAssignableNotSerializable& rhs) const noexcept
    {
        return q == rhs.q && w == rhs.w;
    }
};

template<typename T = Dummy>
class SimpleAssignableSerializable : public csp::ISerializable<GetCrtpMainType<SimpleAssignableSerializable<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableSerializable<T>, T>;
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 2;
    static constexpr uint32_t kInterfaceVersion = 2;         // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] uint8_t& getI()                                                                 noexcept { return m_i; }
    [[nodiscard]] const uint8_t& getI()                                                     const noexcept { return m_i; }
    [[nodiscard]] uint16_t& getJ()                                                                noexcept { return m_j; }
    [[nodiscard]] const uint16_t& getJ()                                                    const noexcept { return m_j; }
    [[nodiscard]] SimpleAssignableAlignedToOneSerializable<>& getSaaToS()                         noexcept { return m_saaToS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneSerializable<>& getSaaToS()             const noexcept { return m_saaToS; }
    [[nodiscard]] SimpleAssignableAlignedToOneNotSerializable& getSaaToNS()                       noexcept { return m_saaToNS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneNotSerializable& getSaaToNS()           const noexcept { return m_saaToNS; }
    [[nodiscard]] SimpleAssignableNotSerializable& getSaNS()                                      noexcept { return m_saNS; }
    [[nodiscard]] const SimpleAssignableNotSerializable& getSaNS()                          const noexcept { return m_saNS; }

    [[nodiscard]] uint32_t* getArrI32()                                                           noexcept { return m_arrI32; }
    [[nodiscard]] const uint32_t* getArrI32()                                               const noexcept { return m_arrI32; }
    [[nodiscard]] size_t getSizeOfArrI32()                                                  const noexcept { return sizeof(m_arrI32); }

    [[nodiscard]] SimpleAssignableAlignedToOneSerializable<>* getArrSaaToS()                      noexcept { return m_arrSaaTos; }
    [[nodiscard]] const SimpleAssignableAlignedToOneSerializable<>* getArrSaaToS()          const noexcept { return m_arrSaaTos; }
    [[nodiscard]] size_t getSizeOfArrSaaToS()                                               const noexcept { return sizeof(m_arrSaaTos); }

    [[nodiscard]] SimpleAssignableAlignedToOneNotSerializable* getArrSaaToNS()                    noexcept { return m_arrSaaToNS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneNotSerializable* getArrSaaToNS()        const noexcept { return m_arrSaaToNS; }
    [[nodiscard]] size_t getSizeOfArrSaaToNS()                                              const noexcept { return sizeof(m_arrSaaToNS); }

    [[nodiscard]] SimpleAssignableNotSerializable* getArrSaNS()                                   noexcept { return m_arrSaNS; }
    [[nodiscard]] const SimpleAssignableNotSerializable* getArrSaNS()                       const noexcept { return m_arrSaNS; }

    [[nodiscard]] bool operator==(const SimpleAssignableSerializable& rhs)                  const noexcept
    {
        return 
               m_i == rhs.m_i
            && m_j == rhs.m_j
            && m_saaToS == rhs.m_saaToS
            && m_saaToNS == rhs.m_saaToNS
            && m_saNS == rhs.m_saNS

            && memcmp(m_arrI32, rhs.m_arrI32, getSizeOfArrI32()) == 0
            && memcmp(m_arrSaaTos, rhs.m_arrSaaTos, getSizeOfArrSaaToS()) == 0
            && memcmp(m_arrSaaToNS, rhs.m_arrSaaToNS, getSizeOfArrSaaToNS()) == 0

            && m_arrSaNS[0] == rhs.m_arrSaNS[0]
            && m_arrSaNS[1] == rhs.m_arrSaNS[1]
            && m_arrSaNS[2] == rhs.m_arrSaNS[2];
    }

private:
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

    friend csp::processing::DataProcessor;
};

template<typename T = Dummy>
struct SimpleAssignableDescendantSerializable : public SimpleAssignableSerializable<GetCrtpMainType<SimpleAssignableDescendantSerializable<T>, T>>
{
    using instance_type = GetCrtpMainType<SimpleAssignableDescendantSerializable<T>, T>;
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 3;
    static constexpr uint32_t kInterfaceVersion = 2;
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    uint32_t v{ 0 };
    
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
        return v == rhs.v && SimpleAssignableSerializable<instance_type>::operator==(rhs);
    }
};

class DynamicPolymorphicNotSerializable
{
public:
    virtual ~DynamicPolymorphicNotSerializable() {}

    static constexpr uint64_t kNameHash = 4;
    static constexpr uint32_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] uint8_t& getR()                 noexcept { return m_r; }    // getters here are only need for testing proposes
    [[nodiscard]] const uint8_t& getR()     const noexcept { return m_r; }    // (not required for serialization itself)
    [[nodiscard]] uint8_t* getArrR()                 noexcept { return m_arrR; }
    [[nodiscard]] const uint8_t* getArrR()     const noexcept { return m_arrR; }
    [[nodiscard]] size_t getSizeOfArrR()       const noexcept { return sizeof(m_arrR); }

    [[nodiscard]] bool operator==(const DynamicPolymorphicNotSerializable& rhs) const noexcept
    {
        return m_r == rhs.m_r && memcmp(m_arrR, rhs.m_arrR, sizeof(m_arrR)) == 0;
    }

private:
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

    static constexpr uint64_t kNameHash = 5;
    static constexpr uint32_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] uint8_t& getO()                 noexcept { return m_o; }    // getters here are only need for testing proposes
    [[nodiscard]] const uint8_t& getO()     const noexcept { return m_o; }    // (not required for serialization itself)
    [[nodiscard]] uint8_t* getArrO()                 noexcept { return m_arrO; }
    [[nodiscard]] const uint8_t* getArrO()     const noexcept { return m_arrO; }
    [[nodiscard]] size_t getSizeOfArrO()       const noexcept { return sizeof(m_arrO); }

    [[nodiscard]] DynamicPolymorphicNotSerializable& getDpNS()                 noexcept { return m_dpNS; }
    [[nodiscard]] const DynamicPolymorphicNotSerializable& getDpNS()     const noexcept { return m_dpNS; }
    [[nodiscard]] DynamicPolymorphicNotSerializable* getArrDpNS()              noexcept { return m_arrDpNS; }
    [[nodiscard]] const DynamicPolymorphicNotSerializable* getArrDpNS()  const noexcept { return m_arrDpNS; }

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

private:
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

    static constexpr uint64_t kNameHash = 8;
    static constexpr uint32_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] bool operator==(const DiamondSerializable& rhs) const noexcept
    {
        return m_d0 == rhs.m_d0 && m_d1 == rhs.m_d1 && m_d2 == rhs.m_d2;
    }

private:
    friend csp::processing::DataProcessor;
};

template<typename T = Dummy>
class SpecialProcessingTypeContainSerializable : public csp::ISerializable<GetCrtpMainType<SpecialProcessingTypeContainSerializable<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<SpecialProcessingTypeContainSerializable<T>, T>;

    static constexpr uint64_t kNameHash = 9;
    static constexpr uint32_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    [[nodiscard]] Vector<DiamondSerializable<>>& getVec()                 noexcept { return m_vec; }    // getters here are only need for testing proposes
    [[nodiscard]] const Vector<DiamondSerializable<>>& getVec()     const noexcept { return m_vec; }    // (not required for serialization itself)

    [[nodiscard]] SimpleAssignableAlignedToOneNotSerializable& getSaaToNS()                       noexcept { return m_saaToNS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneNotSerializable& getSaaToNS()           const noexcept { return m_saaToNS; }
    [[nodiscard]] SimpleAssignableNotSerializable& getSaNS()                                      noexcept { return m_saNS; }
    [[nodiscard]] const SimpleAssignableNotSerializable& getSaNS()                          const noexcept { return m_saNS; }
    [[nodiscard]] Vector<DiamondSerializable<>>*& getPVec()                                       noexcept { return m_pVec; }
    [[nodiscard]] const Vector<DiamondSerializable<>>*& getPVec()                           const noexcept { return m_pVec; }
    [[nodiscard]] const int*& getPInt()                                                           noexcept { return m_pInt; }
    [[nodiscard]] const int* const & getPInt()                                              const noexcept { return m_pInt; }
    [[nodiscard]] const int**& getPpInt()                                                         noexcept { return m_ppInt; }
    [[nodiscard]] const int* const *& getPpInt()                                            const noexcept { return m_ppInt; }

    [[nodiscard]] bool operator==(const SpecialProcessingTypeContainSerializable& rhs) const noexcept
    {
        return m_vec == rhs.m_vec
            && m_saaToNS == rhs.m_saaToNS
            && m_saNS == rhs.m_saNS
            && *m_pVec == *rhs.m_pVec   // this claass only for testing proposes, so there is no redundant nullptr checks
            && **m_ppInt == **m_ppInt;
    }

private:
    Vector<DiamondSerializable<>> m_vec;
    SimpleAssignableAlignedToOneNotSerializable m_saaToNS;
    SimpleAssignableNotSerializable m_saNS;
    Vector<DiamondSerializable<>>* m_pVec = nullptr;
    const int* m_pInt = nullptr;
    const int** m_ppInt = nullptr;

    friend csp::processing::DataProcessor;
};

} // namespace special_types
