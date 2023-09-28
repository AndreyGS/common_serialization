/**
 * @file SpecialTypes.h
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

#include <string>
#include "Status.h"
#include "Allocators/AllocatorHelpers/StrategicAllocatorHelper.h"
#include "Serialization/ISerializable.h"

namespace special_types
{

template<typename T>
using DefaultAllocatorHelper = common_serialization::StrategicAllocatorHelper<T, common_serialization::ConstructorNoexceptAllocator<T>>;

class NoMoveConstructible
{
public:
    char* p = nullptr;
    size_t size = 0;

    NoMoveConstructible() noexcept
    {
        p = new char[1];
        *p = 0;
    }

    NoMoveConstructible(const char* in_p)
    {
        size = strlen(in_p);
        p = new char[size + 1];
        memcpy(p, in_p, size + 1);
    }

    NoMoveConstructible(const NoMoveConstructible& rhs)
    {
        p = new char[rhs.size + 1];
        memcpy(p, rhs.p, rhs.size + 1);
        size = rhs.size;
    }

    NoMoveConstructible& operator=(const NoMoveConstructible& rhs)
    {
        if (this != &rhs)
        {
            delete[] p;

            p = new char[rhs.size + 1];
            memcpy(p, rhs.p, rhs.size + 1);
            size = rhs.size;
        }
        return *this;
    }

    ~NoMoveConstructible()
    {
        delete[] p;
        p = nullptr;
    }

    bool operator==(const NoMoveConstructible& rhs) const noexcept
    {
        if (p && rhs.p)
            return size == rhs.size && memcmp(p, rhs.p, size) == 0;
        else
            return false;
    }
};

struct PodStruct
{
    int i = 0;

    PodStruct() noexcept { }

    PodStruct(const char* p) noexcept
    {
        i = *reinterpret_cast<const int*>(p);
    }

    bool operator ==(const PodStruct& rhs) const noexcept
    {
        return i == rhs.i;
    }
};

class ErrorProne
{
public:
    static uint32_t counter;
    static uint32_t errorOnCounter;
    static common_serialization::Status currentError;

    common_serialization::Status Init(const ErrorProne&)
    {
        if (errorOnCounter == counter++)
            return currentError;
        else
            return common_serialization::Status::kNoError;
    }
};

inline uint32_t ErrorProne::counter = 0;
inline uint32_t ErrorProne::errorOnCounter = 0;
inline common_serialization::Status ErrorProne::currentError = common_serialization::Status::kNoError;


struct EmptyTypeNotSerializable
{
    using empty_type = std::true_type;
};

template<typename T = common_serialization::Dummy>
class EmptyTypeSerializable : public common_serialization::ISerializable<EmptyTypeSerializable<T>>
{
public:
    using empty_type = std::true_type;
    using instance_type = std::conditional_t<std::is_same_v<T, common_serialization::Dummy>, EmptyTypeSerializable<T>, T>;

    static constexpr uint64_t kNameHash = 0;
    static constexpr uint32_t kVersionThis = 0;              // in which version of interface definition of this struct changed
    static constexpr uint32_t kVersionInterface = 0;         // latest version among all dependable struct
};

#pragma pack(push, 1)

struct SimpleAssignableAlignedToOneNotSerializable
{
    using simple_assignable = std::true_type;

    uint16_t a{ 0 };
    uint8_t s{ 0 };

    bool operator==(const SimpleAssignableAlignedToOneNotSerializable& rhs) const noexcept
    {
        return a == rhs.a && s == rhs.s;
    }
};

template<typename T = common_serialization::Dummy>
class SimpleAssignableAlignedToOneSerializable : public common_serialization::ISerializable<SimpleAssignableAlignedToOneSerializable<T>>
{
public:
    using instance_type = std::conditional_t<std::is_same_v<T, common_serialization::Dummy>, SimpleAssignableAlignedToOneSerializable<T>, T>;
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 1;
    static constexpr uint32_t kVersionThis = 0;                 // in which version of interface definition of this struct changed
    static constexpr uint32_t kVersionInterface = 0;            // latest version among all dependable struct

    uint8_t& getX()                 noexcept { return m_x; }    // getters here are only need for testing proposes
    const uint8_t& getX()     const noexcept { return m_x; }    // (not required for serialization itself)
    uint8_t& getY()                 noexcept { return m_y; }
    const uint8_t& getY()     const noexcept { return m_y; }

    bool operator==(const SimpleAssignableAlignedToOneSerializable& rhs) const noexcept
    {
        return m_x == rhs.m_x && m_y == rhs.m_y;
    }

private:
    uint8_t m_x{ 0 };
    uint8_t m_y{ 0 };
};

#pragma pack(pop)

struct SimpleAssignableNotSerializable
{
    using simple_assignable = std::true_type;

    uint16_t q{ 0 };
    uint64_t w{ 0 };

    bool operator==(const SimpleAssignableNotSerializable& rhs) const noexcept
    {
        return q == rhs.q && w == rhs.w;
    }
};

template<typename T = common_serialization::Dummy>
class SimpleAssignableSerializable : public common_serialization::ISerializable<SimpleAssignableSerializable<T>>
{
public:
    // insance_type alias need only in first descendant (maybe it can be used in some other scenarios - should test it)
    using instance_type = std::conditional_t<std::is_same_v<T, common_serialization::Dummy>, SimpleAssignableSerializable<T>, T>;
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 2;
    static constexpr uint32_t kVersionThis = 0;              // in which version of interface definition of this struct changed
    static constexpr uint32_t kVersionInterface = 0;         // latest version among all dependable struct

    uint8_t& getI()                                                                 noexcept { return m_i; }
    const uint8_t& getI()                                                     const noexcept { return m_i; }
    uint16_t& getJ()                                                                noexcept { return m_j; }
    const uint16_t& getJ()                                                    const noexcept { return m_j; }
    SimpleAssignableAlignedToOneSerializable<>& getSaaToS()                         noexcept { return m_saaToS; }
    const SimpleAssignableAlignedToOneSerializable<>& getSaaToS()             const noexcept { return m_saaToS; }
    SimpleAssignableAlignedToOneNotSerializable& getSaaToNS()                       noexcept { return m_saaToNS; }
    const SimpleAssignableAlignedToOneNotSerializable& getSaaToNS()           const noexcept { return m_saaToNS; }
    SimpleAssignableNotSerializable& getSaNS()                                      noexcept { return m_saNS; }
    const SimpleAssignableNotSerializable& getSaNS()                          const noexcept { return m_saNS; }

    uint32_t* getArrI32()                                                           noexcept { return m_arrI32; }
    const uint32_t* getArrI32()                                               const noexcept { return m_arrI32; }
    size_t getSizeOfArrI32()                                                  const noexcept { return sizeof(m_arrI32); }

    SimpleAssignableAlignedToOneSerializable<>* getArrSaaToS()                      noexcept { return m_arrSaaTos; }
    const SimpleAssignableAlignedToOneSerializable<>* getArrSaaToS()          const noexcept { return m_arrSaaTos; }
    size_t getSizeOfArrSaaToS()                                               const noexcept { return sizeof(m_arrSaaTos); }

    SimpleAssignableAlignedToOneNotSerializable* getArrSaaToNS()                    noexcept { return m_arrSaaToNS; }
    const SimpleAssignableAlignedToOneNotSerializable* getArrSaaToNS()        const noexcept { return m_arrSaaToNS; }
    size_t getSizeOfArrSaaToNS()                                              const noexcept { return sizeof(m_arrSaaToNS); }

    SimpleAssignableNotSerializable* getArrSaNS()                                   noexcept { return m_arrSaNS; }
    const SimpleAssignableNotSerializable* getArrSaNS()                       const noexcept { return m_arrSaNS; }

    bool operator==(const SimpleAssignableSerializable& rhs)                  const noexcept
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
};

template<typename T = common_serialization::Dummy>
struct SimpleAssignableDescendantSerializable : public SimpleAssignableSerializable<SimpleAssignableDescendantSerializable<T>>
{
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 3;
    static constexpr uint32_t kVersionThis = 0;
    static constexpr uint32_t kVersionInterface = 0;

    uint32_t v{ 0 };

    SimpleAssignableSerializable<common_serialization::Dummy>& getSimpleAssignableSerializable() noexcept
    {
        return *static_cast<SimpleAssignableSerializable<common_serialization::Dummy>*>(
                    static_cast<void*>(
                        static_cast<SimpleAssignableSerializable<SimpleAssignableDescendantSerializable<T>>*>(this)));
    }

    const SimpleAssignableSerializable<common_serialization::Dummy>& getSimpleAssignableSerializable() const noexcept
    {
        return *static_cast<const SimpleAssignableSerializable<common_serialization::Dummy>*>(
                    static_cast<const void*>(
                        static_cast<const SimpleAssignableSerializable<SimpleAssignableDescendantSerializable<T>>*>(this)));
    }

    bool operator==(const SimpleAssignableDescendantSerializable& rhs) const noexcept
    {
        return v == rhs.v && getSimpleAssignableSerializable() == rhs.getSimpleAssignableSerializable();
    }
};

} // namespace special_types
