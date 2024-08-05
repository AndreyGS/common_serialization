/**
 * @file UnitTests/SerializableStructs/WithStdIncludedInterface/include/with_std_included_interface/Interface.h
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

#include <string>
#include <vector>
#include <map>
#include <tuple>

namespace with_std_included_interface
{

namespace cs = common_serialization;

constexpr cs::csp::Interface properties(
    cs::Uuid{ 0x81ea8b21, 0xd3eb, 0x4e21, 0xa7f6, 0xab5f2449ce66 }
    , 0
    , cs::csp::context::DataFlags{}
    , cs::csp::context::DataFlags{}
);

} // namespace with_std_included_interface

#include <with_std_included_interface/Structs.h>
#include <with_std_included_interface/processing/Generated/Serialize.h>
#include <with_std_included_interface/processing/Generated/Deserialize.h>
