/**
 * @file cslib/include/common_serialization/Containers/DefaultDeleter.h
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

namespace common_serialization
{

template<typename T>
struct DefaultDeleter
{
    constexpr DefaultDeleter() noexcept = default;

    template<typename T2>
        requires std::is_convertible_v<T2*, T*>
    constexpr DefaultDeleter(const DefaultDeleter<T2>&) noexcept {}

    constexpr void operator()(T* p) const noexcept
    {
        static_assert(0 < sizeof(T), "Incomplete types are not allowed to be deleted");
        delete p;
    }
};

template<typename T>
struct DefaultDeleter<T[]>
{
    constexpr DefaultDeleter() noexcept = default;

    template<typename T2>
        requires std::is_convertible_v<T2*, T*>
    constexpr DefaultDeleter(const DefaultDeleter<T2[]>&) noexcept {}

    template<typename T2>
        requires std::is_convertible_v<T2*, T*>
    constexpr void operator()(T2* p) const noexcept
    {
        static_assert(0 < sizeof(T2), "Incomplete types are not allowed to be deleted");
        delete[] p;
    }
};

} // namespace common_serialization
