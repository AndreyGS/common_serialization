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
};

template<typename T = common_serialization::Dummy>
class SimpleAssignableAlignedToOneSerializable : public common_serialization::ISerializable<SimpleAssignableAlignedToOneSerializable<T>>
{
public:
    using instance_type = std::conditional_t<std::is_same_v<T, common_serialization::Dummy>, SimpleAssignableAlignedToOneSerializable<T>, T>;
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 1;
    static constexpr uint32_t kVersionThis = 0;             // in which version of interface definition of this struct changed
    static constexpr uint32_t kVersionInterface = 0;        // latest version among all dependable struct

    uint8_t& getX() { return m_x; }                         // getters here are only need for testing proposes
    uint8_t& getY() { return m_y; }                         // (not required for serialization itself)

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
};

template<typename T = common_serialization::Dummy>
class SimpleAssignableSerializable : public common_serialization::ISerializable<SimpleAssignableSerializable<T>>
{
public:
    using instance_type = std::conditional_t<std::is_same_v<T, common_serialization::Dummy>, SimpleAssignableSerializable<T>, T>;
    using simple_assignable = std::true_type;

    static constexpr uint64_t kNameHash = 2;
    static constexpr uint32_t kVersionThis = 0;              // in which version of interface definition of this struct changed
    static constexpr uint32_t kVersionInterface = 0;         // latest version among all dependable struct

    uint8_t& getI() { return m_i; }
    uint16_t& getJ() { return m_j; }
    SimpleAssignableAlignedToOneSerializable<>& getSaaToS() { return m_saaToS; }
    SimpleAssignableAlignedToOneNotSerializable& getSaaToNS() { return m_saaToNS; }
    SimpleAssignableNotSerializable& getSaNS() { return m_saNS; }

    uint32_t* getArrI32() { return m_arrI32; }
    size_t getSizeOfArrI32() { return sizeof(m_arrI32); }
    SimpleAssignableAlignedToOneSerializable<>* getArrSaaToS() { return m_arrSaaTos; }
    size_t getSizeOfArrSaaToS() { return sizeof(m_arrSaaTos); }
    SimpleAssignableAlignedToOneNotSerializable* getArrSaaToNS() { return m_arrSaaToNS; }
    size_t getSizeOfArrSaaToNS() { return sizeof(m_arrSaaToNS); }
    SimpleAssignableNotSerializable* getArrSaNS() { return m_arrSaNS; }

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

} // namespace special_types
