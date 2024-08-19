/**
 * @file common_serialization/csp_messaging/IServerDataHandlerTraits.h
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

#include <common_serialization/csp_base/types.h>

namespace common_serialization::csp::messaging
{

/// @brief traits_ of CSP Server data handler
template<
      ISerializableImpl _InputType
    , ISerializableImpl _OutputType
    , bool _forTempUseHeap
    , bool _multicast
    , interface_version_t _minimumInterfaceVersion
>
struct IServerDataHandlerTraits
{
    using InputType = _InputType;
    using OutputType = _OutputType;

    static constexpr bool kForTempUseHeap = _forTempUseHeap;
    static constexpr bool kMulticast = _multicast;
    static constexpr interface_version_t kMinimumInterfaceVersion = _minimumInterfaceVersion;
};

template<typename _T>
concept IServerDataHandlerTraitsImpl = std::is_base_of_v<IServerDataHandlerTraits<typename _T::InputType, typename _T::OutputType, _T::kForTempUseHeap, _T::kMulticast, _T::kMinimumInterfaceVersion>, normalize_t<_T>>;

template<ISerializableImpl _InputType, ISerializableImpl _OutputType>
struct MinimumInterfaceVersion
{
    static constexpr interface_version_t value
        = _InputType::getOriginPrivateVersion() < _OutputType::getOriginPrivateVersion()
            ? _InputType::getOriginPrivateVersion()
            : _OutputType::getOriginPrivateVersion();
};

template<
      ISerializableImpl _InputType
    , ISerializableImpl _OutputType
    , interface_version_t _minimumInterfaceVersion = MinimumInterfaceVersion< _InputType, _OutputType>::value
>
using ServerStackHandler = IServerDataHandlerTraits<_InputType, _OutputType, false, false, _minimumInterfaceVersion>;

template<
      ISerializableImpl _InputType
    , ISerializableImpl _OutputType
    , interface_version_t _minimumInterfaceVersion = MinimumInterfaceVersion< _InputType, _OutputType>::value
>
using ServerStackMultiHandler = IServerDataHandlerTraits<_InputType, _OutputType, false, true, _minimumInterfaceVersion>;

template<
      ISerializableImpl _InputType
    , ISerializableImpl _OutputType
    , interface_version_t _minimumInterfaceVersion = MinimumInterfaceVersion< _InputType, _OutputType>::value
>
using ServerHeapHandler = IServerDataHandlerTraits<_InputType, _OutputType, true, false, _minimumInterfaceVersion>;

template<
      ISerializableImpl _InputType
    , ISerializableImpl _OutputType
    , interface_version_t _minimumInterfaceVersion = MinimumInterfaceVersion< _InputType, _OutputType>::value
>
using ServerHeapMultiHandler = IServerDataHandlerTraits<_InputType, _OutputType, true, true, _minimumInterfaceVersion>;

} // namespace common_serialization::csp::messaging
