/**
 * @file common_serialization/tests_restricted_structs/src/structs.cpp
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

#include <common_serialization/tests_restricted_structs/structs.h>

namespace tests_restricted_structs
{

void SimplyAssignableAlignedToOne::fill()
{
    m_a = 1;
    m_s = 2;
}

void SimplyAssignable::fill()
{
    m_q = 1;
    m_w = 2;
}

void DynamicPolymorphic::fill()
{
    m_r = 10;
    memcpy(m_arrR, "abc", sizeof(m_arrR));
}

void DiamondBase::fill()
{
    m_d0 = 18;
}


void DiamondEdge1::fill()
{
    DiamondBase::fill();
    m_d1 = 19;
}

void DiamondEdge2::fill()
{
    DiamondBase::fill();
    m_d2 = 20;
}

void TwoInts::fill()
{
    m_x = 3;
    m_y = 4;
}

void SimplyAssignableWithoutSerializationFunctions::fill()
{
    m_i = 104;
}

} // namespace tests_restricted_structs
