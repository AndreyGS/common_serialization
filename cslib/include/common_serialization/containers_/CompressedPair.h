/**
 * @file cslib/include/common_serialization/containers_/CompressedPair.h
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

#include <common_serialization/common_/common.h>

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

template<typename _T1, typename _T2, bool = std::is_empty_v<_T1> && !std::is_final_v<_T2>>
class CompressedPair final : private _T1
{
public:
    _T2 value;

    using base = _T1;

    template<typename... _T2Params>
    constexpr explicit CompressedPair(zero_then_variadic_args_t, _T2Params&&... t2params) noexcept
        : _T1(), value(std::forward<_T2Params>(t2params)...)
    {
    }

    template<typename _T1Param, typename... _T2Params>
    constexpr CompressedPair(one_then_variadic_args_t, _T1Param&& t1param, _T2Params&&... t2params) noexcept
        : _T1(std::forward<_T1Param>(t1param)), value(std::forward<_T2Params>(t2params)...)
    {
    }

    constexpr _T1& getFirst() noexcept
    {
        return *this;
    }

    constexpr const _T1& getFirst() const noexcept
    {
        return *this;
    }
};

template<typename _T1, typename _T2>
class CompressedPair<_T1, _T2, false>
{
public:
    _T1 baseValue;
    _T2 value;

    using base = _T1;

    template<typename... _T2Params>
    constexpr explicit CompressedPair(zero_then_variadic_args_t, _T2Params&&... t2params) noexcept
        : baseValue(), value(std::forward<_T2Params>(t2params)...)
    {
    }

    template<typename _T1Param, typename... _T2Params>
    constexpr CompressedPair(one_then_variadic_args_t, _T1Param&& t1param, _T2Params&&... t2params) noexcept
        : baseValue(std::forward<_T1Param>(t1param)), value(std::forward<_T2Params>(t2params)...)
    {
    }

    constexpr _T1& getFirst() noexcept
    {
        return baseValue;
    }

    constexpr const _T1& getFirst() const noexcept
    {
        return baseValue;
    }
};

} // namespace common_serialization
