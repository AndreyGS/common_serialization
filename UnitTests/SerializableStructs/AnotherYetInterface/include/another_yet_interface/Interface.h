/**
 * @file UnitTests/SerializableStructs/AnotherYetInterface/include/another_yet_interface/Interface.h
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

 // another_yet_interface UUID  : {f7cbb63c-2a5d-44eb-80fc-591bd08941e2}
 // interface version           : 0

#include "common_serialization/common_serialization.h"

#include "another_yet_interface/Structs.h"
#include "another_yet_interface/Generated/SerializeData.h"
#include "another_yet_interface/Generated/DeserializeData.h"

namespace another_yet_interface
{

namespace cs = common_serialization;

constexpr cs::csp::traits::Interface properties(cs::helpers::getUuid(0xf7cbb63c, 0x2a5d, 0x44eb, 0x80fc, 0x591bd08941e2), 0);

} // namespace another_yet_interface
