/**
 * @file cslib/include/common_serialization/csp/messaging/ClientTraits.h
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

#include "common_serialization/Containers/Concepts.h"
#include "common_serialization/csp/Concepts.h"

namespace common_serialization::csp::messaging
{

/// @brief Properties of CSP Client
template<typename _T>
concept ClientTraits
    =  std::is_same_v<const bool, decltype(_T::kForTempUseHeap)>
    && std::is_same_v<const bool, decltype(_T::kForTempUseHeapExt)>
    && SdContainers<typename _T::Sdcs>;

template<
      bool _forTempUseHeap
    , bool _forTempUseHeapExtended
    , SdContainers _Sdcs
>
struct ClientTraitsConcrete
{
    using Sdcs = _Sdcs;

    static constexpr bool kForTempUseHeap = _forTempUseHeap;
    static constexpr bool kForTempUseHeapExt = _forTempUseHeapExtended;
};

template<SdContainers _Sdcs = traits::DefaultSdContainers>
using ClientStackT = ClientTraitsConcrete<false, false, _Sdcs>;

template<SdContainers _Sdcs = traits::DefaultSdContainers>
using ClientHeapT = ClientTraitsConcrete<true, false, _Sdcs>;

template<SdContainers _Sdcs = traits::DefaultSdContainers>
using ClientHeapExtT = ClientTraitsConcrete<true, true, _Sdcs>;

} // namespace common_serialization::csp::messaging
