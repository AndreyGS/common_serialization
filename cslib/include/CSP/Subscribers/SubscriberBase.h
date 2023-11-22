/**
 * @file SubscriberBase.h
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

#include "SubscribersKeeper.h"

namespace common_serialization
{

namespace csp
{

class SubscriberBase
{
public:
    virtual Status handleDataCommon(Walker<uint8_t>& binInput, Vector<uint8_t>& binOutput) = 0;

    virtual interface_version_t getMinimumHandlerSupportedInterfaceVersion() = 0;
    /*
    virtual name_hash_t         getInputTypeNameHash() = 0;
    virtual interface_version_t*getInputTypeVersionsHierarchy() = 0;
    virtual interface_version_t getInputTypeVersionsHierarchySize() = 0;

    virtual name_hash_t         getOutputTypeNameHash() = 0;
    virtual interface_version_t* getOutputTypeVersionsHierarchy() = 0;
    virtual interface_version_t getOutputTypeVersionsHierarchySize() = 0;*/
};
    
} // namespace csp

} // namespace common_serialization