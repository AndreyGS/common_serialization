/**
 * @file UnitTests/serializable_structs/descendant_interface/include/descendant_interface/Interface.h
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

#include <interface_for_test/Interface.h>

namespace descendant_interface
{

namespace cs = common_serialization;

constexpr cs::csp::Interface properties(
      cs::Uuid{ 0x2335b43f, 0x0228, 0x411b, 0x814f, 0x3393a3e8ac3d }
    , 1
    , cs::csp::context::DataFlags{}
    , cs::csp::context::DataFlags{}
);

} // namespace descendant_interface

#include <descendant_interface/Structs.h>
#include <descendant_interface/processing/Generated/Serialize.h>
#include <descendant_interface/processing/Generated/Deserialize.h>