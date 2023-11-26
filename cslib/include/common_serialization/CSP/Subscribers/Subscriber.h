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

#include "SubscriberBase.h"

namespace common_serialization
{

namespace csp
{

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion = InputType::getMinimumInterfaceVersion()
    , bool handleOnStack = false
    , bool multicast = false
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
class Subscriber : public SubscriberBase
{
public:
    Status handleDataCommon(BinWalker& binInput, BinVector& binOutput) override;

    // It is a default implementation replacement for static handlers
    virtual Status handleData(const InputType& input, OutputType& output);

    [[nodiscard]] interface_version_t getMinimumHandlerSupportedInterfaceVersion() override;

protected:
    Subscriber();
    ~Subscriber();

private:
    Status handleDataOnStack(BinWalker& binInput, BinVector& binOutput);
    Status handleDataOnHeap(BinWalker& binInput, BinVector& binOutput);

    /*
    name_hash_t         getInputTypeNameHash() override;
    interface_version_t*getInputTypeVersionsHierarchy() override;
    interface_version_t getInputTypeVersionsHierarchySize() override;

    name_hash_t         getOutputTypeNameHash() override;
    interface_version_t*getOutputTypeVersionsHierarchy() override;
    interface_version_t getOutputTypeVersionsHierarchySize() override;*/
};

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
Status Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::handleDataCommon(BinWalker& binInput, BinVector& binOutput)
{
    if constexpr (handleOnStack)
        return handleDataOnStack(binInput, binOutput);
    else
        return handleDataOnHeap(binInput, binOutput);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::handleData(const InputType& input, OutputType& output)
{
    assert(false);
    return Status::kErrorNoSuchHandler;
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
interface_version_t Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::getMinimumHandlerSupportedInterfaceVersion()
{
    return minimumInterfaceVersion;
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::Subscriber()
{
    GetSubscribersManager().addSubscriber(InputType::getNameHash(), multicast, this);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::~Subscriber()
{
    GetSubscribersManager().removeSubscriber(InputType::getNameHash(), this);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::handleDataOnStack(BinWalker& binInput, BinVector& binOutput)
{
    InputType input;
    OutputType output;
    //RuN(input.deserialize(binInput));

    if constexpr (std::is_same_v<InstanceType, Dummy>)
        return this->handleData(input, output);
    else
        return InstanceType::handleDataStatic(input, output);
}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
Status Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::handleDataOnHeap(BinWalker& binInput, BinVector& binOutput)
{
    return Status::kErrorNoSuchHandler;
}

/*
template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
name_hash_t Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::getInputTypeNameHash()
{

}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
interface_version_t* Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::getInputTypeVersionsHierarchy()
{

}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
interface_version_t Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::getInputTypeVersionsHierarchySize()
{

}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
name_hash_t Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::getOutputTypeNameHash()
{

}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
interface_version_t* Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::getOutputTypeVersionsHierarchy()
{

}

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion, bool handleOnStack, bool multicast
>
    requires IsISerializableBased<InputType>&& IsISerializableBased<OutputType>
interface_version_t Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>::getOutputTypeVersionsHierarchySize()
{

}*/

template<typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion = InputType::getMinimumInterfaceVersion()
    , bool handleOnStack = false
    , bool multicast = false
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
using MethodSubscriber = Subscriber<Dummy, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>;

template<typename InstanceType, typename InputType, typename OutputType
    , interface_version_t minimumInterfaceVersion = InputType::getMinimumInterfaceVersion()
    , bool handleOnStack = false
    , bool multicast = false
>
    requires IsISerializableBased<InputType> && IsISerializableBased<OutputType>
using StaticSubscriber = Subscriber<InstanceType, InputType, OutputType, minimumInterfaceVersion, handleOnStack, multicast>;

} // namespace csp

} // namespace common_serialization