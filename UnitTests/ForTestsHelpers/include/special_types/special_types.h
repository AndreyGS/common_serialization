/**
 * @file UnitTests/ForTestsHelpers/include/special_types/special_types.h
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

#include "common_serialization/common_serialization.h"

namespace special_types
{

namespace cs = common_serialization;

template<typename T>
using DefaultAllocatorHelper = cs::StrategicAllocatorHelper<T, cs::ConstructorNoexceptAllocator<T>>;

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
    static cs::Status currentError;

    cs::Status init(const ErrorProne&)
    {
        if (errorOnCounter == counter++)
            return currentError;
        else
            return cs::Status::kNoError;
    }
};

inline uint32_t ErrorProne::counter = 0;
inline uint32_t ErrorProne::errorOnCounter = 0;
inline cs::Status ErrorProne::currentError = cs::Status::kNoError;

#pragma pack(push, 1)

struct SimplyAssignableAlignedToOneNotInterfaceType
{
    char j;
    int k;

    using simply_assignable_aligned_to_one_tag = std::true_type;
};

#pragma pack(pop)

struct SimplyAssignableNotInterfaceType
{
    char j;
    int k;

    using simply_assignable_tag = std::true_type;
};

struct SimplyAssignableTypeWithoutTypeUsingNotInterfaceType
{
    char j;
    short k;
};

struct EmptyTypeWithoutTypeUsingNotInterfaceType
{

};

} // namespace special_types
