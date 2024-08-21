/**
 * @file common_serialization/tests_special_types/structs.h
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

#include <string>
#include <type_traits>
#include <common_serialization/common/common.h>

namespace tests_special_types
{

namespace ags_cs = common_serialization;

class NoMoveConstructible
{
public:
    std::string m_str;

    NoMoveConstructible() = default;

    NoMoveConstructible(const char* in_p)
    {
        m_str = in_p;
    }

    NoMoveConstructible(const NoMoveConstructible& rhs) = default;
    NoMoveConstructible& operator=(const NoMoveConstructible& rhs) = default;

    ~NoMoveConstructible() = default;

    [[nodiscard]] auto operator<=>(const NoMoveConstructible&) const = default;
};

struct PodStruct
{
    int m_i{ 0 };

    PodStruct() = default;

    explicit PodStruct(int i) : m_i(i) { }

    PodStruct(const char* p) noexcept
    {
        m_i = *reinterpret_cast<const int*>(p);
    }

    constexpr [[nodiscard]] auto operator<=>(const PodStruct&) const = default;

    static void operator delete(PodStruct* p, std::destroying_delete_t)
    {

    }
};

struct PodStructDesc : PodStruct
{
    PodStructDesc() = default;
    PodStructDesc(int i) : PodStruct(i) { }
};

class ErrorProne
{
public:
    static uint32_t counter;
    static uint32_t errorOnCounter;
    static ags_cs::Status currentError;
    static uint32_t destructorCalledCounter;

    ags_cs::Status init(const ErrorProne&)
    {
        if (errorOnCounter == counter++)
            return currentError;
        else
            return ags_cs::Status::NoError;
    }

    ~ErrorProne()
    {
        ++destructorCalledCounter;
    }
};

inline uint32_t ErrorProne::counter = 0;
inline uint32_t ErrorProne::errorOnCounter = 0;
inline ags_cs::Status ErrorProne::currentError = ags_cs::Status::NoError;
inline uint32_t ErrorProne::destructorCalledCounter = 0;

struct CustomDeleter
{
    template<typename _T>
    void operator()(_T* p)
    {
        delete p;
    }
};

struct CustomDeleter2 : CustomDeleter
{
};

struct CustomArrDeleter
{
    template<typename _T>
    void operator()(_T* p)
    {
        delete[] p;
    }
};

struct CustomArrDeleter2 : CustomArrDeleter
{
};

struct CustomDeleterStruct
{
    std::string m_name;
    int m_i{ 0 };

    CustomDeleterStruct() = default;

    explicit CustomDeleterStruct(int i) : m_i(i), m_name("CustomDeleterStruct") { }

    [[nodiscard]] auto operator<=>(const CustomDeleterStruct&) const = default;

    static void operator delete(CustomDeleterStruct* p, std::destroying_delete_t) noexcept;

protected:
    CustomDeleterStruct(int i, const std::string& name) : m_i(i), m_name(name) { }
};

struct CustomDeleterStructDesc : CustomDeleterStruct
{
    CustomDeleterStructDesc() = default;
    explicit CustomDeleterStructDesc(int i) : CustomDeleterStruct(i, "CustomDeleterStructDesc") { }
};

inline void CustomDeleterStruct::operator delete(CustomDeleterStruct * p, std::destroying_delete_t) noexcept
{
    if (!p)
        return;

    if (p->m_name == "CustomDeleterStruct")
        ::delete p;
    else
        ::delete reinterpret_cast<CustomDeleterStructDesc*>(p);
}

struct VirtDistructorOwner
{
    int i{ 0 };

    VirtDistructorOwner() = default;

    // it must not be explicit for tests reason
    VirtDistructorOwner(int i) : i(i) {}

    virtual ~VirtDistructorOwner() {}
};

struct VirtDistructorOwnerDesc : VirtDistructorOwner
{
    VirtDistructorOwnerDesc() = default;

    // it must not be explicit for tests reason
    VirtDistructorOwnerDesc(int i) : VirtDistructorOwner(i) {}
};

} // namespace tests_special_types
