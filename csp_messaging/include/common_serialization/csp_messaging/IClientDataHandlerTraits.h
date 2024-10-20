/**
 * @file common_serialization/csp_messaging/IClientDataHandlerTraits.h
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

namespace common_serialization::csp::messaging
{

/// @brief traits_ of CSP Client data handler
template<
      ISerializableImpl InputType_
    , ISerializableImpl OutputType_
    , bool forTempUseHeap
>
struct IClientDataHandlerTraits
{
    using InputType = InputType_;
    using OutputType = OutputType_;

    static constexpr bool kForTempUseHeap = forTempUseHeap;
};

template<typename T>
concept IClientDataHandlerTraitsImpl = std::is_base_of_v<IClientDataHandlerTraits<typename T::InputType, typename T::OutputType, T::kForTempUseHeap>, normalize_t<T>>;

template<ISerializableImpl InputType, ISerializableImpl OutputType>
using ClientStackHandler = IClientDataHandlerTraits<InputType, OutputType, false>;

template<ISerializableImpl InputType, ISerializableImpl OutputType>
using ClientHeapHandler = IClientDataHandlerTraits<InputType, OutputType, true>;

} // namespace common_serialization::csp::messaging
