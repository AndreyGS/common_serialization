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

    using simple_assignable_tag = std::true_type;
};

template<typename T = Dummy>
class SimpleAssignableAlignedToOneSerializable_Version0 : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version0<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version0<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr uint64_t kNameHash = 11;
    static constexpr uint32_t kInterfaceVersion = 0;            // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    SimpleAssignableAlignedToOneSerializable_Version0() { }
    template<typename T2>
    Status init(const SimpleAssignableAlignedToOneSerializable_Version1<T2>& rhs);

    [[nodiscard]] bool operator==(const SimpleAssignableAlignedToOneSerializable_Version0& rhs) const noexcept
    {
        return m_ti == rhs.m_ti;
    }

    TwoInts m_ti{ 0 };

private:
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

    static constexpr uint64_t kNameHash = 2;
    static constexpr uint32_t kInterfaceVersion = 0;         // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    template<typename T2>
    Status init(const SimpleAssignableSerializable<T2>& rhs);

    [[nodiscard]] uint8_t& getI()                                                                 noexcept { return m_i; }
    [[nodiscard]] uint8_t getI()                                                            const noexcept { return m_i; }
    [[nodiscard]] uint16_t& getJ()                                                                noexcept { return m_j; }
    [[nodiscard]] uint16_t getJ()                                                           const noexcept { return m_j; }
    [[nodiscard]] SimpleAssignableAlignedToOneSerializable_Version0<>& getSaaToS()                noexcept { return m_saaToS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneSerializable_Version0<>& getSaaToS()    const noexcept { return m_saaToS; }
    [[nodiscard]] SimpleAssignableAlignedToOneNotSerializable& getSaaToNS()                       noexcept { return m_saaToNS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneNotSerializable& getSaaToNS()           const noexcept { return m_saaToNS; }
    [[nodiscard]] SimpleAssignableNotSerializable& getSaNS()                                      noexcept { return m_saNS; }
    [[nodiscard]] const SimpleAssignableNotSerializable& getSaNS()                          const noexcept { return m_saNS; }

    [[nodiscard]] uint32_t* getArrI32()                                                           noexcept { return m_arrI32; }
    [[nodiscard]] const uint32_t* getArrI32()                                               const noexcept { return m_arrI32; }
    [[nodiscard]] size_t getSizeOfArrI32()                                                  const noexcept { return sizeof(m_arrI32); }

    [[nodiscard]] SimpleAssignableAlignedToOneSerializable_Version0<>* getArrSaaToS()             noexcept { return m_arrSaaTos; }
    [[nodiscard]] const SimpleAssignableAlignedToOneSerializable_Version0<>* getArrSaaToS() const noexcept { return m_arrSaaTos; }
    [[nodiscard]] size_t getSizeOfArrSaaToS()                                               const noexcept { return sizeof(m_arrSaaTos); }

    [[nodiscard]] SimpleAssignableAlignedToOneNotSerializable* getArrSaaToNS()                    noexcept { return m_arrSaaToNS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneNotSerializable* getArrSaaToNS()        const noexcept { return m_arrSaaToNS; }
    [[nodiscard]] size_t getSizeOfArrSaaToNS()                                              const noexcept { return sizeof(m_arrSaaToNS); }

    [[nodiscard]] SimpleAssignableNotSerializable* getArrSaNS()                                   noexcept { return m_arrSaNS; }
    [[nodiscard]] const SimpleAssignableNotSerializable* getArrSaNS()                       const noexcept { return m_arrSaNS; }

    [[nodiscard]] uint32_t& getVt()                                                               noexcept { return m_vt; }
    [[nodiscard]] uint32_t getVt()                                                          const noexcept { return m_vt; }

    SimpleAssignableSerializable_Version0& operator=(const SimpleAssignableSerializable_Version0<>& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        m_i = rhs.m_i;
        m_j = rhs.m_j;
        m_saaToS = rhs.m_saaToS;
        m_saaToNS = rhs.m_saaToNS;
        m_saNS = rhs.m_saNS;
        memcpy(m_arrI32, rhs.m_arrI32, getSizeOfArrI32());
        memcpy(m_arrSaaTos, rhs.m_arrSaaTos, getSizeOfArrI32());
        memcpy(m_arrSaaToNS, rhs.m_arrSaaToNS, getSizeOfArrI32());
        memcpy(m_arrSaNS, rhs.m_arrSaNS, sizeof(m_arrSaNS));
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

            && memcmp(m_arrI32, rhs.m_arrI32, getSizeOfArrI32()) == 0
            && memcmp(m_arrSaaTos, rhs.m_arrSaaTos, getSizeOfArrSaaToS()) == 0
            && memcmp(m_arrSaaToNS, rhs.m_arrSaaToNS, getSizeOfArrSaaToNS()) == 0

            && m_arrSaNS[0] == rhs.m_arrSaNS[0]
            && m_arrSaNS[1] == rhs.m_arrSaNS[1]
            && m_arrSaNS[2] == rhs.m_arrSaNS[2]
            
            && m_vt == rhs.m_vt
            ;
    }

private:
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
    m_saaToS = rhs.m_saaToS;
    m_saaToNS = rhs.m_saaToNS;
    m_saNS = rhs.m_saNS;
    memcpy(m_arrI32, rhs.m_arrI32, getSizeOfArrI32());
    memcpy(m_arrSaaTos, rhs.m_arrSaaTos, getSizeOfArrI32());
    memcpy(m_arrSaaToNS, rhs.m_arrSaaToNS, getSizeOfArrI32());
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
    m_saaToS = rhs.m_saaToS;
    m_saaToNS = rhs.m_saaToNS;
    m_saNS = rhs.m_saNS;
    memcpy(m_arrI32, rhs.m_arrI32, getSizeOfArrI32());
    memcpy(m_arrSaaTos, rhs.m_arrSaaTos, getSizeOfArrI32());
    memcpy(m_arrSaaToNS, rhs.m_arrSaaToNS, getSizeOfArrI32());
    memcpy(m_arrSaNS, rhs.m_arrSaNS, sizeof(m_arrSaNS));

    m_vx = rhs.m_vt * 2;

    return Status::kNoError;
}

template<typename T = Dummy>
struct SimpleAssignableDescendantSerializable_Version0 : public SimpleAssignableSerializable_Version0<GetCrtpMainType<SimpleAssignableDescendantSerializable_Version0<T>, T>>
{
    using instance_type = GetCrtpMainType<SimpleAssignableDescendantSerializable_Version0<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr uint64_t kNameHash = 3;
    static constexpr uint32_t kInterfaceVersion = 0;
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

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
    SimpleAssignableSerializable_Version0<T2>::init(rhs);
    m_d = rhs.m_d;
}

template<typename T1>
template<typename T2>
Status SimpleAssignableDescendantSerializable<T1>::init(const SimpleAssignableDescendantSerializable_Version0<T2>& rhs)
{
    SimpleAssignableSerializable<T2>::init(rhs);
    m_d = rhs.m_d;
}

template<typename>
class ForAllFlagsTests1_Version2;

template<typename T = Dummy>
class ForAllFlagsTests1_Version0 : public csp::ISerializable<GetCrtpMainType<ForAllFlagsTests1_Version0<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<ForAllFlagsTests1_Version0<T>, T>;

    static constexpr uint64_t kNameHash = 10000;
    static constexpr uint32_t kInterfaceVersion = 0;
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    template<typename T2>
    Status init(const ForAllFlagsTests1_Version2<T2>& rhs);

    [[nodiscard]] SimpleAssignableDescendantSerializable_Version0<>& getSaDs()                        noexcept { return m_saDs; }
    [[nodiscard]] const SimpleAssignableDescendantSerializable_Version0<>& getSaDs()            const noexcept { return m_saDs; }
    [[nodiscard]] DiamondSerializable<>& getDiamond()                                                 noexcept { return m_diamond; }
    [[nodiscard]] const DiamondSerializable<>& getDiamond()                                     const noexcept { return m_diamond; }
    [[nodiscard]] SpecialProcessingTypeContainSerializable<>& getSptCs()                              noexcept { return m_sptCs; }
    [[nodiscard]] const SpecialProcessingTypeContainSerializable<>& getSptCs()                  const noexcept { return m_sptCs; }
    [[nodiscard]] SimpleAssignableAlignedToOneSimilarType1Serializable<>& getSaaToStS()               noexcept { return m_saaToStS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneSimilarType1Serializable<>& getSaaToStS()   const noexcept { return m_saaToStS; }
    [[nodiscard]] SimpleAssignableSimilarType1Serializable<>& getSaStS()                              noexcept { return m_saStS; }
    [[nodiscard]] const SimpleAssignableSimilarType1Serializable<>& getSaStS()                  const noexcept { return m_saStS; }
    [[nodiscard]] SimilarType1Serializable<>& getStS()                                                noexcept { return m_stS; }
    [[nodiscard]] const SimilarType1Serializable<>& getStS()                                    const noexcept { return m_stS; }
    [[nodiscard]] ManyPointersTypeSerializable<>& getMpT()                                            noexcept { return m_mpt; }
    [[nodiscard]] const ManyPointersTypeSerializable<>& getMpT()                                const noexcept { return m_mpt; }

    [[nodiscard]] bool operator==(const ForAllFlagsTests1_Version0<>& rhs) const noexcept
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

private:
    DiamondSerializable<> m_diamond;
    SpecialProcessingTypeContainSerializable<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType1Serializable<> m_saaToStS;
    SimpleAssignableSimilarType1Serializable<> m_saStS;
    SimilarType1Serializable<> m_stS;
    ManyPointersTypeSerializable<> m_mpt;

    SimpleAssignableDescendantSerializable_Version0<> m_saDs;

    friend csp::processing::DataProcessor;
    friend ForAllFlagsTests1_Version2;
};


template<typename>
class ForAllFlagsTests2_Version2;

template<typename T = Dummy>
class ForAllFlagsTests2_Version0 : public csp::ISerializable<GetCrtpMainType<ForAllFlagsTests2_Version0<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<ForAllFlagsTests2_Version0<T>, T>;

    static constexpr uint64_t kNameHash = 10000;
    static constexpr uint32_t kInterfaceVersion = 0;
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    template<typename T2>
    Status init(const ForAllFlagsTests2_Version2<T2>& rhs);

    [[nodiscard]] SimpleAssignableDescendantSerializable_Version0<>& getSaDs()                                 noexcept { return m_saDs; }
    [[nodiscard]] const SimpleAssignableDescendantSerializable_Version0<>& getSaDs()                     const noexcept { return m_saDs; }
    [[nodiscard]] DiamondSerializable<>& getDiamond()                                                 noexcept { return m_diamond; }
    [[nodiscard]] const DiamondSerializable<>& getDiamond()                                     const noexcept { return m_diamond; }
    [[nodiscard]] SpecialProcessingTypeContainSerializable<>& getSptCs()                              noexcept { return m_sptCs; }
    [[nodiscard]] const SpecialProcessingTypeContainSerializable<>& getSptCs()                  const noexcept { return m_sptCs; }
    [[nodiscard]] SimpleAssignableAlignedToOneSimilarType1Serializable<>& getSaaToStS()               noexcept { return m_saaToStS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneSimilarType1Serializable<>& getSaaToStS()   const noexcept { return m_saaToStS; }
    [[nodiscard]] SimpleAssignableSimilarType1Serializable<>& getSaStS()                              noexcept { return m_saStS; }
    [[nodiscard]] const SimpleAssignableSimilarType1Serializable<>& getSaStS()                  const noexcept { return m_saStS; }
    [[nodiscard]] SimilarType1Serializable<>& getStS()                                                noexcept { return m_stS; }
    [[nodiscard]] const SimilarType1Serializable<>& getStS()                                    const noexcept { return m_stS; }
    [[nodiscard]] ManyPointersTypeSerializable<>& getMpT()                                            noexcept { return m_mpt; }
    [[nodiscard]] const ManyPointersTypeSerializable<>& getMpT()                                const noexcept { return m_mpt; }

    [[nodiscard]] bool operator==(const ForAllFlagsTests2_Version0<>& rhs) const noexcept
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

private:
    DiamondSerializable<> m_diamond;
    SpecialProcessingTypeContainSerializable<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType2Serializable<> m_saaToStS;
    SimpleAssignableSimilarType2Serializable<> m_saStS;
    SimilarType2Serializable<> m_stS;
    ManyPointersTypeSerializable<> m_mpt;

    SimpleAssignableDescendantSerializable_Version0<> m_saDs;

    friend csp::processing::DataProcessor;
    friend ForAllFlagsTests2_Version2;
};


template<typename T = Dummy>
class SimpleAssignableAlignedToOneSerializable_Version1 : public csp::ISerializable<GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version1<T>, T>>
{
public:
    using instance_type = GetCrtpMainType<SimpleAssignableAlignedToOneSerializable_Version1<T>, T>;
    using simple_assignable_tag = std::true_type;

    static constexpr uint64_t kNameHash = 10;
    static constexpr uint32_t kInterfaceVersion = 1;            // latest version among all dependable structs
    static constexpr uint32_t kVersionsHierarchy[] = { 1, 0 };

    [[nodiscard]] uint8_t& getX()                 noexcept { return m_x; }    // getters here are only need for testing proposes
    [[nodiscard]] const uint8_t& getX()     const noexcept { return m_x; }    // (not required for serialization itself)
    [[nodiscard]] uint8_t& getY()                 noexcept { return m_y; }
    [[nodiscard]] const uint8_t& getY()     const noexcept { return m_y; }

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

private:
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
class ForAllFlagsTests1_Version2 : public csp::ISerializable<GetCrtpMainType<ForAllFlagsTests1_Version2<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<ForAllFlagsTests1_Version2<T>, T>;

    static constexpr uint64_t kNameHash = 10000;
    static constexpr uint32_t kInterfaceVersion = 2;
    static constexpr uint32_t kVersionsHierarchy[] = { 2, 0 };

    template<typename T2>
    Status init(const ForAllFlagsTests1_Version0<T2>& rhs);
    template<typename T2>
    Status init(const ForAllFlagsTests1<T2>& rhs);

    [[nodiscard]] SimpleAssignableSerializable<>& getSaS()                                            noexcept { return m_saS; }
    [[nodiscard]] const SimpleAssignableSerializable<>& getSaS()                                const noexcept { return m_saS; }
    [[nodiscard]] int& getI()                                                                         noexcept { return m_i; }
    [[nodiscard]] int getI()                                                                    const noexcept { return m_i; }

    [[nodiscard]] DiamondSerializable<>& getDiamond()                                                 noexcept { return m_diamond; }
    [[nodiscard]] const DiamondSerializable<>& getDiamond()                                     const noexcept { return m_diamond; }
    [[nodiscard]] SpecialProcessingTypeContainSerializable<>& getSptCs()                              noexcept { return m_sptCs; }
    [[nodiscard]] const SpecialProcessingTypeContainSerializable<>& getSptCs()                  const noexcept { return m_sptCs; }
    [[nodiscard]] SimpleAssignableAlignedToOneSimilarType1Serializable<>& getSaaToStS()               noexcept { return m_saaToStS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneSimilarType1Serializable<>& getSaaToStS()   const noexcept { return m_saaToStS; }
    [[nodiscard]] SimpleAssignableSimilarType1Serializable<>& getSaStS()                              noexcept { return m_saStS; }
    [[nodiscard]] const SimpleAssignableSimilarType1Serializable<>& getSaStS()                  const noexcept { return m_saStS; }
    [[nodiscard]] SimilarType1Serializable<>& getStS()                                                noexcept { return m_stS; }
    [[nodiscard]] const SimilarType1Serializable<>& getStS()                                    const noexcept { return m_stS; }
    [[nodiscard]] ManyPointersTypeSerializable<>& getMpT()                                            noexcept { return m_mpt; }
    [[nodiscard]] const ManyPointersTypeSerializable<>& getMpT()                                const noexcept { return m_mpt; }

    [[nodiscard]] bool operator==(const ForAllFlagsTests1_Version2<>& rhs) const noexcept
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

private:
    DiamondSerializable<> m_diamond;
    SpecialProcessingTypeContainSerializable<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType1Serializable<> m_saaToStS;
    SimpleAssignableSimilarType1Serializable<> m_saStS;
    SimilarType1Serializable<> m_stS;
    ManyPointersTypeSerializable<> m_mpt;

    SimpleAssignableSerializable<> m_saS;
    int m_i{ 0 }; // duplicated m_saDs.m_i

    friend csp::processing::DataProcessor;
    friend ForAllFlagsTests1_Version0;
    friend ForAllFlagsTests1;
};

template<typename T1>
template<typename T2>
Status ForAllFlagsTests1_Version0<T1>::init(const ForAllFlagsTests1_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saDs = rhs.m_saS;
    m_saDs.getI() += 1; // additional conversion

    return Status;
}

template<typename T1>
template<typename T2>
Status ForAllFlagsTests1_Version2<T1>::init(const ForAllFlagsTests1_Version0<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saS = rhs.m_saDs;
    m_saS.getI() -= 1; // additional conversion
    m_i = rhs.m_saDs.getI();
}

template<typename T1>
template<typename T2>
Status ForAllFlagsTests1_Version2<T1>::init(const ForAllFlagsTests1<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saS = rhs.m_saDs;
    m_saS.getI() += 1; // additional conversion
    m_i = rhs.m_saDs.v;

    return Status;
}

template<typename T1>
template<typename T2>
Status ForAllFlagsTests1<T1>::init(const ForAllFlagsTests1_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saDs = rhs.m_saS;
    m_saDs.getI() -= 1; // additional conversion
    rhs.m_saDs.v = rhs.m_i;

    return Status;
}

template<typename T = Dummy>
class ForAllFlagsTests2_Version2 : public csp::ISerializable<GetCrtpMainType<ForAllFlagsTests2_Version2<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<ForAllFlagsTests2_Version2<T>, T>;

    static constexpr uint64_t kNameHash = 10000;
    static constexpr uint32_t kInterfaceVersion = 2;
    static constexpr uint32_t kVersionsHierarchy[] = { 2, 0 };

    template<typename T2>
    Status init(const ForAllFlagsTests2_Version0<T2>& rhs);
    template<typename T2>
    Status init(const ForAllFlagsTests2<T2>& rhs);

    [[nodiscard]] SimpleAssignableSerializable<>& getSaS()                                            noexcept { return m_saS; }
    [[nodiscard]] const SimpleAssignableSerializable<>& getSaS()                                const noexcept { return m_saS; }
    [[nodiscard]] int& getI()                                                                         noexcept { return m_i; }
    [[nodiscard]] int getI()                                                                    const noexcept { return m_i; }

    [[nodiscard]] DiamondSerializable<>& getDiamond()                                                 noexcept { return m_diamond; }
    [[nodiscard]] const DiamondSerializable<>& getDiamond()                                     const noexcept { return m_diamond; }
    [[nodiscard]] SpecialProcessingTypeContainSerializable<>& getSptCs()                              noexcept { return m_sptCs; }
    [[nodiscard]] const SpecialProcessingTypeContainSerializable<>& getSptCs()                  const noexcept { return m_sptCs; }
    [[nodiscard]] SimpleAssignableAlignedToOneSimilarType1Serializable<>& getSaaToStS()               noexcept { return m_saaToStS; }
    [[nodiscard]] const SimpleAssignableAlignedToOneSimilarType1Serializable<>& getSaaToStS()   const noexcept { return m_saaToStS; }
    [[nodiscard]] SimpleAssignableSimilarType1Serializable<>& getSaStS()                              noexcept { return m_saStS; }
    [[nodiscard]] const SimpleAssignableSimilarType1Serializable<>& getSaStS()                  const noexcept { return m_saStS; }
    [[nodiscard]] SimilarType1Serializable<>& getStS()                                                noexcept { return m_stS; }
    [[nodiscard]] const SimilarType1Serializable<>& getStS()                                    const noexcept { return m_stS; }
    [[nodiscard]] ManyPointersTypeSerializable<>& getMpT()                                            noexcept { return m_mpt; }
    [[nodiscard]] const ManyPointersTypeSerializable<>& getMpT()                                const noexcept { return m_mpt; }

    [[nodiscard]] bool operator==(const ForAllFlagsTests2_Version2<>& rhs) const noexcept
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

private:
    DiamondSerializable<> m_diamond;
    SpecialProcessingTypeContainSerializable<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType2Serializable<> m_saaToStS;
    SimpleAssignableSimilarType2Serializable<> m_saStS;
    SimilarType2Serializable<> m_stS;
    ManyPointersTypeSerializable<> m_mpt;

    SimpleAssignableSerializable<> m_saS;
    int m_i{ 0 }; // duplicated m_saDs.m_i

    friend csp::processing::DataProcessor;
    friend ForAllFlagsTests2_Version0;
    friend ForAllFlagsTests2;
};

template<typename T1>
template<typename T2>
Status ForAllFlagsTests2_Version0<T1>::init(const ForAllFlagsTests2_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saDs = rhs.m_saS;
    m_saDs.getI() += 1; // additional conversion

    return Status;
}

template<typename T1>
template<typename T2>
Status ForAllFlagsTests2_Version2<T1>::init(const ForAllFlagsTests2_Version0<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saS = rhs.m_saDs;
    m_saS.getI() -= 1; // additional conversion
    m_i = rhs.m_saDs.getI();

    return Status;
}

template<typename T1>
template<typename T2>
Status ForAllFlagsTests2_Version2<T1>::init(const ForAllFlagsTests2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saS = rhs.m_saDs;
    m_saS.getI() += 1; // additional conversion
    m_i = rhs.m_saDs.v;

    return Status;
}

template<typename T1>
template<typename T2>
Status ForAllFlagsTests2<T1>::init(const ForAllFlagsTests2_Version2<T2>& rhs)
{
    m_diamond = rhs.m_diamond;
    m_sptCs = rhs.m_sptCs;
    m_saaToStS = rhs.m_saaToStS;
    m_saStS = rhs.m_saStS;
    m_stS = rhs.m_stS;
    m_mpt = rhs.m_mpt;

    m_saDs = rhs.m_saS;
    m_saDs.getI() -= 1; // additional conversion
    rhs.m_saDs.v = rhs.m_i;

    return Status;
}

} // namespace special_types
