/**
 * @file SpecialTypesFillingStruct.cpp
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

#include "SpecialTypesFillingStruct.h"

namespace special_types
{

template<>
void fillingStruct(SimpleAssignableAlignedToOneNotSerializable& output)
{
    output.a = 1;
    output.s = 2;
}

template<>
void fillingStruct(SimpleAssignableAlignedToOneSerializable<>& output)
{
    output.getX() = 3;
    output.getY() = 4;
}

template<>
void fillingStruct(SimpleAssignableNotSerializable& output)
{
    output.q = 5;
    output.w = 6;
}

template<>
void fillingStruct(SimpleAssignableSerializable<>& output)
{
    output.getI() = 7;
    output.getJ() = 8;

    fillingStruct(output.getSaaToS());
    fillingStruct(output.getSaaToNS());
    fillingStruct(output.getSaNS());

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

template<>
void fillingStruct(SimpleAssignableDescendantSerializable<>& output)
{
    fillingStruct<SimpleAssignableSerializable<>>(output);
    output.v = 9;
}

template<>
void fillingStruct(DynamicPolymorphicNotSerializable& output)
{
    output.getR() = 10;
    memcpy(output.getArrR(), "abc", output.getSizeOfArrR());
}

template<>
void fillingStruct(DynamicPolymorphicSerializable<>& output)
{
    output.getO() = 11;
    fillingStruct(output.getDpNS());

    memcpy(output.getArrO(), "def", output.getSizeOfArrO());

    fillingStruct(*output.getArrDpNS());

    output.getArrDpNS()[1].getR() = 12;
    memcpy(output.getArrDpNS()[1].getArrR(), "ghi", output.getArrDpNS()[1].getSizeOfArrR());
    output.getArrDpNS()[2].getR() = 13;
    memcpy(output.getArrDpNS()[2].getArrR(), "jkl", output.getArrDpNS()[2].getSizeOfArrR());
}

template<>
void fillingStruct(DiamondBaseNotSerializable& output)
{
    output.m_d0 = 18;
}

template<>
void fillingStruct(DiamondEdge1NotSerializable& output)
{
    fillingStruct<DiamondBaseNotSerializable>(output);
    output.m_d1 = 19;
}

template<>
void fillingStruct(DiamondEdge2NotSerializable& output)
{
    fillingStruct<DiamondBaseNotSerializable>(output);
    output.m_d2 = 20;
}

template<>
void fillingStruct(DiamondSerializable<>& output)
{
    fillingStruct<DiamondEdge1NotSerializable>(output);
    fillingStruct<DiamondEdge2NotSerializable>(output);
}

template<>
void fillingStruct(SpecialProcessingTypeContainSerializable<>& output)
{
    DiamondSerializable ds1;
    fillingStruct(ds1);
    output.getVec().pushBack(ds1);

    DiamondSerializable ds2;
    ds2.m_d0 = 23;
    ds2.m_d0 = 24;
    ds2.m_d0 = 25;

    output.getVec().pushBack(ds2);
    fillingStruct(output.getSaaToNS());
    fillingStruct(output.getSaNS());

    output.getPVec() = new Vector<DiamondSerializable<>>;
    *output.getPVec() = output.getVec();
    output.getPInt() = new int;
    *const_cast<int*&>(output.getPInt()) = 64984561;

    output.getPpInt() = new const int*;
    *output.getPpInt() = new int;
    **const_cast<int**&>(output.getPpInt()) = 548505248;
}

template<>
void fillingStruct(SimpleAssignableAlignedToOneSimilarType1Serializable<>& output)
{
    output.setJ(99);
    output.setK(98352739);
}

template<>
void fillingStruct(SimpleAssignableAlignedToOneSimilarType2Serializable<>& output)
{
    output.setJ(100);
    output.setK(15456);
}

template<>
void fillingStruct(SimpleAssignableSimilarType1Serializable<>& output)
{
    output.setJ(101);
    output.setK(8582727);
}

template<>
void fillingStruct(SimpleAssignableSimilarType2Serializable<>& output)
{
    output.setJ(100);
    output.setK(8374);
}

template<>
void fillingStruct(SimilarType1Serializable<>& output)
{
    output.setJ(101);
    output.setK(8582727);
}

template<>
void fillingStruct(SimilarType2Serializable<>& output)
{
    output.setJ(100);
    output.setK(8374);
}

template<>
void fillingStruct(ManyPointersTypeSerializable<>& output)
{
    Vector<int*, StrategicRawNoexceptAllocatorHelper<int*>> vec;
    int* pInt = new int[3] { 1, 2, 3};
    vec.pushBack(pInt);
    vec.pushBack(pInt + 1);
    vec.pushBack(pInt + 2);

    output.getVec() = vec;
    output.getVecRec().pushBack(&output.getVec());
    output.getPVec() = &output.getVec();

    output.getRTSpec1().pAny = &output.getRTSpec2();
    output.getRTSpec2().pI = pInt;
    output.getRTSpec2().pAny = &output.getRTSpec1();
    output.getRTSpec2().pNext = &output.getRTSpec2();
    
    output.getPInt() = pInt;
    memcpy(output.getIntArr(), pInt, 3 * sizeof(int));
    output.getPpInt() = &output.getPInt();
}

template<>
void fillingStruct(ForAllFlagsTests1_Version0<>& output)
{
    fillingStruct(output.getSaDs());

    fillingStruct(output.getDiamond());
    fillingStruct(output.getSptCs());
    fillingStruct(output.getSaaToStS());
    fillingStruct(output.getSaStS());
    fillingStruct(output.getStS());
    fillingStruct(output.getMpT());
}

template<>
void fillingStruct(ForAllFlagsTests1_Version1<>& output)
{
    fillingStruct(output.getSaS());
    output.getI() = 185468;

    fillingStruct(output.getDiamond());
    fillingStruct(output.getSptCs());
    fillingStruct(output.getSaaToStS());
    fillingStruct(output.getSaStS());
    fillingStruct(output.getStS());
    fillingStruct(output.getMpT());
}

template<>
void fillingStruct(ForAllFlagsTests1<>& output)
{
    fillingStruct(output.getSaDs());

    fillingStruct(output.getDiamond());
    fillingStruct(output.getSptCs());
    fillingStruct(output.getSaaToStS());
    fillingStruct(output.getSaStS());
    fillingStruct(output.getStS());
    fillingStruct(output.getMpT());
}

} // namespace special_types
