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
    output.m_x = 3;
    output.m_y = 4;
}

template<>
void fillingStruct(SimpleAssignableAlignedToOneSerializable_Version0<>& output)
{
    output.m_ti.x = 17;
    output.m_ti.y = 18654;
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
    output.m_i = 7;
    output.m_j = 8;

    fillingStruct(output.m_saaToS);
    fillingStruct(output.m_saaToNS);
    fillingStruct(output.m_saNS);

    memcpy(output.m_arrI32, "123456789012", sizeof(output.m_arrI32));
    memcpy(output.m_arrSaaTos, "00001000034000056", sizeof(output.m_arrSaaTos));
    memcpy(output.m_arrSaaToNS, "123456789", sizeof(output.m_arrSaaToNS));

    // we shouldn't memcpy array of SimpleAssignableNotSerializable cause its not aligned by one
    output.m_arrSaNS[0].q = 123;
    output.m_arrSaNS[0].w = 456;
    output.m_arrSaNS[1].q = 567;
    output.m_arrSaNS[1].w = 890;
    output.m_arrSaNS[2].q = 789;
    output.m_arrSaNS[2].w = 012;

    output.m_vx = 8974;  // must be even (for tests)
}

template<>
void fillingStruct(SimpleAssignableSerializable_Version0<>& output)
{
    output.m_i = 7;
    output.m_j = 8;

    fillingStruct(output.m_saaToS);
    fillingStruct(output.m_saaToNS);
    fillingStruct(output.m_saNS);

    memcpy(output.m_arrI32, "123456789012", sizeof(output.m_arrI32));
    memcpy(output.m_arrSaaTos, "00001000034000056", sizeof(output.m_arrSaaTos));
    memcpy(output.m_arrSaaToNS, "123456789", sizeof(output.m_arrSaaToNS));

    // we shouldn't memcpy array of SimpleAssignableNotSerializable cause its not aligned by one
    output.m_arrSaNS[0].q = 123;
    output.m_arrSaNS[0].w = 456;
    output.m_arrSaNS[1].q = 567;
    output.m_arrSaNS[1].w = 890;
    output.m_arrSaNS[2].q = 789;
    output.m_arrSaNS[2].w = 012;

    output.m_vt = 46984;
}

template<>
void fillingStruct(SimpleAssignableDescendantSerializable_Version0<>& output)
{
    fillingStruct<SimpleAssignableSerializable_Version0<>>(output);
    output.m_d = 9;
}

template<>
void fillingStruct(SimpleAssignableDescendantSerializable<>& output)
{
    fillingStruct<SimpleAssignableSerializable<>>(output);
    output.m_d = 9;
}

template<>
void fillingStruct(DynamicPolymorphicNotSerializable& output)
{
    output.m_r = 10;
    memcpy(output.m_arrR, "abc", sizeof(output.m_arrR));
}

template<>
void fillingStruct(DynamicPolymorphicSerializable<>& output)
{
    output.m_o = 11;
    fillingStruct(output.m_dpNS);

    memcpy(output.m_arrO, "def", sizeof(output.m_arrO));

    fillingStruct(*output.m_arrDpNS);

    output.m_arrDpNS[1].m_r = 12;
    memcpy(output.m_arrDpNS[1].m_arrR, "ghi", sizeof(output.m_arrDpNS[1].m_arrR));
    output.m_arrDpNS[2].m_r = 13;
    memcpy(output.m_arrDpNS[2].m_arrR, "jkl", sizeof(output.m_arrDpNS[2].m_arrR));
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
    output.m_vec.pushBack(ds1);

    DiamondSerializable ds2;
    ds2.m_d0 = 23;
    ds2.m_d0 = 24;
    ds2.m_d0 = 25;

    output.m_vec.pushBack(ds2);
    fillingStruct(output.m_saaToNS);
    fillingStruct(output.m_saNS);

    output.m_pVec = new Vector<DiamondSerializable<>>;
    *output.m_pVec = output.m_vec;
    output.m_pInt = new int;
    *const_cast<int*&>(output.m_pInt) = 64984561;

    output.m_ppInt = new const int*;
    *output.m_ppInt = new int;
    **const_cast<int**&>(output.m_ppInt) = 548505248;
}

template<>
void fillingStruct(SimpleAssignableAlignedToOneSimilarType1Serializable<>& output)
{
    output.m_j = 99;
    output.m_k = 98352739;
}

template<>
void fillingStruct(SimpleAssignableAlignedToOneSimilarType2Serializable<>& output)
{
    output.m_j = 100;
    output.m_k = 15456;
}

template<>
void fillingStruct(SimpleAssignableSimilarType1Serializable<>& output)
{
    output.m_j = 101;
    output.m_k = 8582727;
}

template<>
void fillingStruct(SimpleAssignableSimilarType2Serializable<>& output)
{
    output.m_j = 102;
    output.m_k = 8374;
}

template<>
void fillingStruct(SimilarType1Serializable<>& output)
{
    output.m_j = 103;
    output.m_k = 8582727;
}

template<>
void fillingStruct(SimilarType2Serializable<>& output)
{
    output.m_j = 104;
    output.m_k = 8374;
}

template<>
void fillingStruct(ManyPointersTypeSerializable<>& output)
{
    Vector<int*, StrategicRawNoexceptAllocatorHelper<int*>> vec;
    int* pInt = new int[3] { 1, 2, 3};
    vec.pushBack(pInt);
    vec.pushBack(pInt + 1);
    vec.pushBack(pInt + 2);

    output.m_vec = vec;
    output.m_vecRecursive.pushBack(&output.m_vec);
    output.m_vecRecursive.pushBack(nullptr);
    output.m_pVec = &output.m_vec;

    output.m_rtSpec1.pAny = &output.m_rtSpec2;
    output.m_rtSpec2.pI = pInt;
    output.m_rtSpec2.pAny = &output.m_rtSpec1;
    output.m_rtSpec2.pNext = &output.m_rtSpec2;
    
    output.m_pInt = pInt;
    memcpy(output.m_intArr, pInt, sizeof(output.m_intArr));
    output.m_ppInt = &output.m_pInt;
}

template<>
void fillingStruct(ForAllFlagsTests1_Version0<>& output)
{
    fillingStruct(output.m_saDs);

    fillingStruct(output.m_diamond);
    fillingStruct(output.m_sptCs);
    fillingStruct(output.m_saaToStS);
    fillingStruct(output.m_saStS);
    fillingStruct(output.m_stS);
    fillingStruct(output.m_mpt);
}

template<>
void fillingStruct(ForAllFlagsTests1_Version2<>& output)
{
    fillingStruct(output.m_saS);
    output.m_i = 185468;

    fillingStruct(output.m_diamond);
    fillingStruct(output.m_sptCs);
    fillingStruct(output.m_saaToStS);
    fillingStruct(output.m_saStS);
    fillingStruct(output.m_stS);
    fillingStruct(output.m_mpt);
}

template<>
void fillingStruct(ForAllFlagsTests1<>& output)
{
    fillingStruct(output.m_saDs);

    fillingStruct(output.m_diamond);
    fillingStruct(output.m_sptCs);
    fillingStruct(output.m_saaToStS);
    fillingStruct(output.m_saStS);
    fillingStruct(output.m_stS);
    fillingStruct(output.m_mpt);
}

} // namespace special_types
