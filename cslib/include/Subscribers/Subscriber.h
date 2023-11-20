/**
 * @file Subscriber.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

#include "SubscribersManager.h"


namespace common_serialization
{

template<typename T, typename InputType, typename OutputType, bool multicast = false>
    requires serialization_concepts::IsISerializableBased<InputType> && serialization_concepts::IsISerializableBased<OutputType>
class Subscriber
{
public:
    constexpr Subscriber();

    static Status handleDataCommon(void* instance, Walker<uint8_t>& binInput, Vector<uint8_t>& binOutput);
};

template<typename T, typename InputType, typename OutputType, bool multicast>
    requires serialization_concepts::IsISerializableBased<InputType> && serialization_concepts::IsISerializableBased<OutputType>
constexpr Subscriber<T, InputType, OutputType, multicast>::Subscriber()
{ 
    GetSubscribersManager().addSubscriber(InputType::getNameHash(), multicast, handleDataCommon, this);
}

template<typename T, typename InputType, typename OutputType, bool multicast>
    requires serialization_concepts::IsISerializableBased<InputType>&& serialization_concepts::IsISerializableBased<OutputType>
Status Subscriber<T, InputType, OutputType, multicast>::handleDataCommon(void* pInstance, Walker<uint8_t>& binInput, Vector<uint8_t>& binOutput)
{
    InputType input;
    OutputType output;
    //RuN(input.deserialize(binInput));

    return static_cast<T*>(pInstance)->handleData(input, output);
}


} // namespace common_serialization