/**
 * @file SpecialTypesFilling.h
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

#include "SerializableStructs/InterfaceForTest/First/Include/Interface.h"

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
    _SimpleAssignableSerializable(output);
    output.v = 9;
}

void _DynamicPolymorphicNotSerializable(DynamicPolymorphicNotSerializable& output)
{
    output.getR() = 10;
    memcpy(output.getArrR(), "abc", output.getSizeOfArrR());
}

void _DynamicPolymorphicSerializable(DynamicPolymorphicSerializable<>& output)
{
    output.getO() = 11;
    _DynamicPolymorphicNotSerializable(output.getDpNS());

    memcpy(output.getArrO(), "def", output.getSizeOfArrO());

    _DynamicPolymorphicNotSerializable(*output.getArrDpNS());

    output.getArrDpNS()[1].getR() = 12;
    memcpy(output.getArrDpNS()[1].getArrR(), "ghi", output.getArrDpNS()[1].getSizeOfArrR());
    output.getArrDpNS()[2].getR() = 13;
    memcpy(output.getArrDpNS()[2].getArrR(), "jkl", output.getArrDpNS()[2].getSizeOfArrR());
}

void _DiamondBaseNotSerializable(DiamondBaseNotSerializable& output)
{
    output.m_d0 = 18;
}

void _DiamondEdge1NotSerializable(DiamondEdge1NotSerializable& output)
{
    _DiamondBaseNotSerializable(output);
    output.m_d1 = 19;
}

void _DiamondEdge2NotSerializable(DiamondEdge2NotSerializable& output)
{
    _DiamondBaseNotSerializable(output);
    output.m_d2 = 20;
}

void _DiamondSerializable(DiamondSerializable<>& output)
{
    _DiamondEdge1NotSerializable(output);
    _DiamondEdge2NotSerializable(output);
}

void _SpecialProcessingTypeContainSerializable(SpecialProcessingTypeContainSerializable<>& output)
{
    DiamondSerializable ds1;
    _DiamondSerializable(ds1);
    output.getVec().pushBack(ds1);

    DiamondSerializable ds2;
    ds2.m_d0 = 23;
    ds2.m_d0 = 24;
    ds2.m_d0 = 25;

    output.getVec().pushBack(ds2);
}

} // namespace filling

} // namespace special_types