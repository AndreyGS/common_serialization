/**
 * @file cslib/include/common_serialization/csp/messaging/service_structs/Interface.h
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

// Service structs is a mandatory part of CSP.
// Set of structs is completely depend on protocol version. 

namespace common_serialization::csp::service_structs
{

namespace cs = common_serialization;

constexpr cs::csp::Interface properties(
      Uuid{ 0xe47be322, 0x232e, 0x4d66, 0x9175, 0x06eed2110b4b }
    , 1
    , context::DataFlags{}
    , context::DataFlags{ context::DataFlags::kSizeOfIntegersMayBeNotEqual | context::DataFlags::kAllowUnmanagedPointers | context::DataFlags::kCheckRecursivePointers }
);

} // namespace common_serialization::csp::service_structs

#include "common_serialization/csp/service_structs/Structs.h"
#include "common_serialization/csp/service_structs/processing/Deserialize.h"
#include "common_serialization/csp/service_structs/processing/Serialize.h"
