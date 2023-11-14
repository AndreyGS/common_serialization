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

private:
    TwoInts m_ti{ 0 };

    friend csp::processing::DataProcessor;
    friend SimpleAssignableAlignedToOneSerializable_Version1;
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
class ForAllFlagsTests1_Version0 : public csp::ISerializable<GetCrtpMainType<ForAllFlagsTests1_Version0<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<ForAllFlagsTests1_Version0<T>, T>;

    static constexpr uint64_t kNameHash = 10000;
    static constexpr uint32_t kInterfaceVersion = 0;
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    template<typename T2>
    Status init(const ForAllFlagsTests1_Version1<T2>& rhs);

    [[nodiscard]] SimpleAssignableDescendantSerializable<>& getSaDs()                                 noexcept { return m_saDs; }
    [[nodiscard]] const SimpleAssignableDescendantSerializable<>& getSaDs()                     const noexcept { return m_saDs; }
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

    SimpleAssignableDescendantSerializable<> m_saDs;

    friend csp::processing::DataProcessor;
    friend ForAllFlagsTests1_Version1;
};

template<typename T = Dummy>
class ForAllFlagsTests2_Version0 : public csp::ISerializable<GetCrtpMainType<ForAllFlagsTests2_Version0<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<ForAllFlagsTests2_Version0<T>, T>;

    static constexpr uint64_t kNameHash = 10000;
    static constexpr uint32_t kInterfaceVersion = 0;
    static constexpr uint32_t kVersionsHierarchy[] = { 0 };

    template<typename T2>
    Status init(const ForAllFlagsTests2_Version1<T2>& rhs);

    [[nodiscard]] SimpleAssignableDescendantSerializable<>& getSaDs()                                 noexcept { return m_saDs; }
    [[nodiscard]] const SimpleAssignableDescendantSerializable<>& getSaDs()                     const noexcept { return m_saDs; }
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
    SimpleAssignableDescendantSerializable<> m_saDs;
    DiamondSerializable<> m_diamond;
    SpecialProcessingTypeContainSerializable<> m_sptCs;
    SimpleAssignableAlignedToOneSimilarType2Serializable<> m_saaToStS;
    SimpleAssignableSimilarType2Serializable<> m_saStS;
    SimilarType2Serializable<> m_stS;
    ManyPointersTypeSerializable<> m_mpt;

    friend csp::processing::DataProcessor;
    friend ForAllFlagsTests2_Version1;
};

template<typename T = Dummy>
class ForAllFlagsTests1_Version1 : public csp::ISerializable<GetCrtpMainType<ForAllFlagsTests1_Version1<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<ForAllFlagsTests1_Version1<T>, T>;

    static constexpr uint64_t kNameHash = 10000;
    static constexpr uint32_t kInterfaceVersion = 1;
    static constexpr uint32_t kVersionsHierarchy[] = { 1, 0 };

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

    [[nodiscard]] bool operator==(const ForAllFlagsTests1_Version1<>& rhs) const noexcept
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
Status ForAllFlagsTests1_Version0<T1>::init(const ForAllFlagsTests1_Version1<T2>& rhs)
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
Status ForAllFlagsTests1_Version1<T1>::init(const ForAllFlagsTests1_Version0<T2>& rhs)
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
Status ForAllFlagsTests1_Version1<T1>::init(const ForAllFlagsTests1<T2>& rhs)
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
Status ForAllFlagsTests1<T1>::init(const ForAllFlagsTests1_Version1<T2>& rhs)
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
class ForAllFlagsTests2_Version1 : public csp::ISerializable<GetCrtpMainType<ForAllFlagsTests2_Version1<T>, T >>
{
public:
    using instance_type = GetCrtpMainType<ForAllFlagsTests2_Version1<T>, T>;

    static constexpr uint64_t kNameHash = 10000;
    static constexpr uint32_t kInterfaceVersion = 1;
    static constexpr uint32_t kVersionsHierarchy[] = { 1, 0 };

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

    [[nodiscard]] bool operator==(const ForAllFlagsTests2_Version1<>& rhs) const noexcept
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
Status ForAllFlagsTests2_Version0<T1>::init(const ForAllFlagsTests2_Version1<T2>& rhs)
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
Status ForAllFlagsTests2_Version1<T1>::init(const ForAllFlagsTests2_Version0<T2>& rhs)
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
Status ForAllFlagsTests2_Version1<T1>::init(const ForAllFlagsTests2<T2>& rhs)
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
Status ForAllFlagsTests2<T1>::init(const ForAllFlagsTests2_Version1<T2>& rhs)
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
