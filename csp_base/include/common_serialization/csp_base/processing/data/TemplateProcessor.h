/**
 * @file common_serialization/csp_base/processing/data/TemplateProcessor.h
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

#include <common_serialization/csp_base/context/Data.h>

namespace common_serialization::csp::processing::data
{

template<typename _T, typename... _Ts>
class TemplateProcessor {};


template<template<typename...> typename _T, typename... _Ts>
CS_ALWAYS_INLINE Status templateProcessorSerializationWrapper(const _T<_Ts...>& value, context::SData& ctx)
{
    return TemplateProcessor<_T<_Ts...>, _Ts...>::serialize(value, ctx);
}


template<template<typename...> typename _T, typename... _Ts>
CS_ALWAYS_INLINE Status templateProcessorDeserializationWrapper(context::DData& ctx, _T<_Ts...>& value)
{
    return TemplateProcessor<_T<_Ts...>, _Ts...>::deserialize(ctx, value);
}

} // namespace common_serialization::csp::processing::data
