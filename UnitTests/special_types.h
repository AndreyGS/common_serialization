#include <string>
#include "Status.h"

namespace special_types
{

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

} // namespace special_types
