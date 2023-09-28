/**
 * @file FillingSpecialTypes.h
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

#include "SpecialTypes.h"

namespace special_types
{

namespace filling
{

void _SimpleAssignableAlignedToOneNotSerializable(SimpleAssignableAlignedToOneNotSerializable& output)
{
    output.a = 1;
    output.s = 2;
}

void _SimpleAssignableAlignedToOneSerializable(SimpleAssignableAlignedToOneSerializable<>& output)
{
    output.getX() = 3;
    output.getY() = 4;
}

void _SimpleAssignableNotSerializable(SimpleAssignableNotSerializable& output)
{
    output.q = 5;
    output.w = 6;
}


void _SimpleAssignableSerializable(SimpleAssignableSerializable<>& output)
{
    output.getI() = 7;
    output.getJ() = 8;

    _SimpleAssignableAlignedToOneSerializable(output.getSaaToS());
    _SimpleAssignableAlignedToOneNotSerializable(output.getSaaToNS());
    _SimpleAssignableNotSerializable(output.getSaNS());

    memcpy(output.getArrI32(), "123456789012", output.getSizeOfArrI32());
    memcpy(output.getArrSaaToS(), "00001000034000056", output.getSizeOfArrSaaToS());
    memcpy(output.getArrSaaToNS(), "123456789", output.getSizeOfArrSaaToNS());

    // we shouldn't memcpy array of SimpleAssignableNotSerializable cause its not aligned by one
    output.getArrSaNS()[0].q = 123;
    output.getArrSaNS()[0].w = 456;
    output.getArrSaNS()[1].q = 567;
    output.getArrSaNS()[1].w = 890;
    output.getArrSaNS()[2].q = 789;
    output.getArrSaNS()[2].w = 012;
}

void _SimpleAssignableDescendantSerializable(SimpleAssignableDescendantSerializable<>& output)
{
    _SimpleAssignableSerializable(output.getSimpleAssignableSerializable());
    output.v = 9;
}




} // namespace filling

} // namespace special_types