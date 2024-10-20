/**
 * @file common_serialization/containers/CompressedPair.h
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

#include <common_serialization/common/common.h>

namespace common_serialization
{

struct zero_then_variadic_args_t 
{
    explicit zero_then_variadic_args_t() = default;
};

struct one_then_variadic_args_t
{
    explicit one_then_variadic_args_t() = default;
};

template<typename T1, typename T2, bool = std::is_empty_v<T1> && !std::is_final_v<T2>>
class CompressedPair final : private T1
{
public:
    T2 value;

    using base = T1;

    template<typename... T2Params>
    constexpr explicit CompressedPair(zero_then_variadic_args_t, T2Params&&... t2params) noexcept
        : T1(), value(std::forward<T2Params>(t2params)...)
    {
    }

    template<typename T1Param, typename... T2Params>
    constexpr CompressedPair(one_then_variadic_args_t, T1Param&& t1param, T2Params&&... t2params) noexcept
        : T1(std::forward<T1Param>(t1param)), value(std::forward<T2Params>(t2params)...)
    {
    }

    constexpr T1& getFirst() noexcept
    {
        return *this;
    }

    constexpr const T1& getFirst() const noexcept
    {
        return *this;
    }
};

template<typename T1, typename T2>
class CompressedPair<T1, T2, false>
{
public:
    T1 baseValue;
    T2 value;

    using base = T1;

    template<typename... T2Params>
    constexpr explicit CompressedPair(zero_then_variadic_args_t, T2Params&&... t2params) noexcept
        : baseValue(), value(std::forward<T2Params>(t2params)...)
    {
    }

    template<typename T1Param, typename... T2Params>
    constexpr CompressedPair(one_then_variadic_args_t, T1Param&& t1param, T2Params&&... t2params) noexcept
        : baseValue(std::forward<T1Param>(t1param)), value(std::forward<T2Params>(t2params)...)
    {
    }

    constexpr T1& getFirst() noexcept
    {
        return baseValue;
    }

    constexpr const T1& getFirst() const noexcept
    {
        return baseValue;
    }
};

} // namespace common_serialization
