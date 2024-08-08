/**
 * @file UnitTests/for_tests_helpers/src/FillingStruct.cpp
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

#include <ft_helpers/FillingStruct.h>

namespace ft_helpers
{

using namespace common_serialization;

template<>
void fillingStruct(restricted_structs::SimplyAssignableAlignedToOne& output)
{
    output.a = 1;
    output.s = 2;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableAlignedToOne_Version0<>& output)
{
    output.m_ti.x = 3;
    output.m_ti.y = 4;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableAlignedToOne<>& output)
{
    output.m_x = 3;
    output.m_y = 4;
}

template<>
void fillingStruct(restricted_structs::SimplyAssignable& output)
{
    output.q = 5;
    output.w = 6;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignable_Version0<>& output)
{
    output.m_i = 7;
    output.m_j = 8;

    fillingStruct(output.m_saaToS);
    fillingStruct(output.m_saaToNS);
    fillingStruct(output.m_saNS);

    memcpy(output.m_arrI32, "123456789012", sizeof(output.m_arrI32));
    output.m_arrSaaTos[0].m_ti.x = 48;
    output.m_arrSaaTos[0].m_ti.y = 60;
    output.m_arrSaaTos[1].m_ti.x = 49;
    output.m_arrSaaTos[1].m_ti.y = 61;
    output.m_arrSaaTos[2].m_ti.x = 50;
    output.m_arrSaaTos[2].m_ti.y = 62;

    output.m_arrSaaToNS[0].a = 15000;
    output.m_arrSaaToNS[0].s = 122;
    output.m_arrSaaToNS[1].a = 15001;
    output.m_arrSaaToNS[1].s = 123;
    output.m_arrSaaToNS[2].a = 15002;
    output.m_arrSaaToNS[2].s = 124;

    output.m_arrSaNS[0].q = 123;
    output.m_arrSaNS[0].w = 456;
    output.m_arrSaNS[1].q = 567;
    output.m_arrSaNS[1].w = 890;
    output.m_arrSaNS[2].q = 789;
    output.m_arrSaNS[2].w = 012;

    output.m_vt = 23492;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignable<>& output)
{
    output.m_i = 7;
    output.m_j = 8;

    fillingStruct(output.m_saaToS);
    fillingStruct(output.m_saaToNS);
    fillingStruct(output.m_saNS);

    memcpy(output.m_arrI32, "123456789012", sizeof(output.m_arrI32));

    output.m_arrSaaTos[0].m_x = 48;
    output.m_arrSaaTos[0].m_y = 60;
    output.m_arrSaaTos[1].m_x = 49;
    output.m_arrSaaTos[1].m_y = 61;
    output.m_arrSaaTos[2].m_x = 50;
    output.m_arrSaaTos[2].m_y = 62;

    output.m_arrSaaToNS[0].a = 15000;
    output.m_arrSaaToNS[0].s = 122;
    output.m_arrSaaToNS[1].a = 15001;
    output.m_arrSaaToNS[1].s = 123;
    output.m_arrSaaToNS[2].a = 15002;
    output.m_arrSaaToNS[2].s = 124;

    output.m_arrSaNS[0].q = 123;
    output.m_arrSaNS[0].w = 456;
    output.m_arrSaNS[1].q = 567;
    output.m_arrSaNS[1].w = 890;
    output.m_arrSaNS[2].q = 789;
    output.m_arrSaNS[2].w = 012;

    output.m_vx = 46984;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableDescendant_Version0<>& output)
{
    fillingStruct<interface_for_test::SimplyAssignable_Version0<>>(output);
    output.m_d = output.m_i + 2;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableDescendant<>& output)
{
    fillingStruct<interface_for_test::SimplyAssignable<>>(output);
    output.m_d = output.m_i + 2;
}

template<>
void fillingStruct(interface_for_test::AlwaysSimplyAssignable_Version0<>& output)
{
    output.m_xx = 211;
    output.m_yy = 117;
}

template<>
void fillingStruct(interface_for_test::AlwaysSimplyAssignable<>& output)
{
    output.m_x = 210;
    output.m_y = 115;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableFixedSize_Version1<>& output)
{
    output.m_xx = 179;
    fillingStruct(output.m_asa);
    output.m_arrAsa[0].m_xx = 31;
    output.m_arrAsa[0].m_yy = 134;
    output.m_arrAsa[1].m_xx = 89;
    output.m_arrAsa[1].m_yy = 55;
    output.m_arrAsa[2].m_xx = 160;
    output.m_arrAsa[2].m_yy = 229;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableFixedSize<>& output)
{
    output.m_x = 178;
    fillingStruct(output.m_asa);
    output.m_arrAsa[0].m_x = 30;
    output.m_arrAsa[0].m_y = 132;
    output.m_arrAsa[1].m_x = 88;
    output.m_arrAsa[1].m_y = 53;
    output.m_arrAsa[2].m_x = 159;
    output.m_arrAsa[2].m_y = 227;
}

template<>
void fillingStruct(restricted_structs::DynamicPolymorphic& output)
{
    output.m_r = 10;
    memcpy(output.m_arrR, "abc", sizeof(output.m_arrR));
}

template<>
void fillingStruct(interface_for_test::DynamicPolymorphic<>& output)
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
void fillingStruct(restricted_structs::DiamondBase& output)
{
    output.m_d0 = 18;
}

template<>
void fillingStruct(restricted_structs::DiamondEdge1& output)
{
    fillingStruct<restricted_structs::DiamondBase>(output);
    output.m_d1 = 19;
}

template<>
void fillingStruct(restricted_structs::DiamondEdge2& output)
{
    fillingStruct<restricted_structs::DiamondBase>(output);
    output.m_d2 = 20;
}

template<>
void fillingStruct(interface_for_test::Diamond<>& output)
{
    fillingStruct<restricted_structs::DiamondEdge1>(output);
    fillingStruct<restricted_structs::DiamondEdge2>(output);
}

template<>
void fillingStruct(interface_for_test::SpecialProcessingType<>& output)
{
    interface_for_test::Diamond ds1;
    fillingStruct(ds1);
    output.m_vec.pushBack(ds1);

    interface_for_test::Diamond ds2;
    ds2.m_d0 = 23;
    ds2.m_d0 = 24;
    ds2.m_d0 = 25;

    output.m_vec.pushBack(ds2);
    fillingStruct(output.m_saaToNS);
    fillingStruct(output.m_saNS);

    output.m_pVec = new Vector<interface_for_test::Diamond<>>;
    *output.m_pVec = output.m_vec;
    output.m_pInt = new int;
    *const_cast<int*&>(output.m_pInt) = 64984561;

    output.m_ppInt = new const int*;
    *output.m_ppInt = new int;
    **const_cast<int**&>(output.m_ppInt) = 548505248;

    output.m_nullptrInt = nullptr;

    output.m_c = -82;
    output.m_sh = 48893;
    output.m_m = cs::csp::context::Message::Data;
    output.m_tEnum = interface_for_test::SpecialProcessingType<>::TEnum::ValN2;
    output.m_ll = -83242399125;
    output.m_float = 0.326236329032f;
    output.m_double = -13523623623.25;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableAlignedToOneSimilarType1<>& output)
{
    output.m_j = 99;
    output.m_k = 15456;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableAlignedToOneSimilarType2<>& output)
{
    output.m_j = 99;
    output.m_k = 15456;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableSimilarType1<>& output)
{
    output.m_j = 101;
    output.m_k = 8374;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableSimilarType2<>& output)
{
    output.m_j = 101;
    output.m_k = 8374;
}

template<>
void fillingStruct(interface_for_test::SimilarType1<>& output)
{
    output.m_j = 102;
    output.m_k = 9951;

    output.m_arrL[0] = 34;
    output.m_arrL[1] = 44;
    output.m_arrL[2] = 54;
    output.m_arrL[3] = 64;
    output.m_arrL[4] = 74;

    output.m_sasTs[0].m_j = 105;
    output.m_sasTs[0].m_k = 11123;
    output.m_sasTs[1].m_j = 100;
    output.m_sasTs[1].m_k = 20110;
    output.m_sasTs[2].m_j = 105;
    output.m_sasTs[2].m_k = 25548;

    output.m_saaToSts[0].m_j = 21;
    output.m_saaToSts[0].m_k = 11923;
    output.m_saaToSts[1].m_j = 85;
    output.m_saaToSts[1].m_k = 31002;
    output.m_saaToSts[2].m_j = 54;
    output.m_saaToSts[2].m_k = 4563;
}

template<>
void fillingStruct(interface_for_test::SimilarType2<>& output)
{
    output.m_j = 102;
    output.m_k = 9951;

    output.m_arrL[0] = 34;
    output.m_arrL[1] = 44;
    output.m_arrL[2] = 54;
    output.m_arrL[3] = 64;
    output.m_arrL[4] = 74;

    output.m_sasTs[0].m_j = 105;
    output.m_sasTs[0].m_k = 11123;
    output.m_sasTs[1].m_j = 100;
    output.m_sasTs[1].m_k = 20110;
    output.m_sasTs[2].m_j = 105;
    output.m_sasTs[2].m_k = 25548;

    output.m_saaToSts[0].m_j = 21;
    output.m_saaToSts[0].m_k = 11923;
    output.m_saaToSts[1].m_j = 85;
    output.m_saaToSts[1].m_k = 31002;
    output.m_saaToSts[2].m_j = 54;
    output.m_saaToSts[2].m_k = 4563;
}

template<>
void fillingStruct(interface_for_test::ManyPointersType<>& output)
{
    RawVectorT<int*> vec;
    // It must be raw pointer, which will be deleted on test cleanup
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
void fillingStruct(interface_for_test::SForAllModesTests_Version0<>& output)
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
void fillingStruct(interface_for_test::SForAllModesTests_Version2<>& output)
{
    fillingStruct(output.m_saS);
    output.m_i = output.m_saS.m_i + 2;      // == SimplyAssignableDescendant::m_d;
    output.m_saS.m_i -= 1;

    fillingStruct(output.m_diamond);
    fillingStruct(output.m_sptCs);
    fillingStruct(output.m_saaToStS);
    fillingStruct(output.m_saStS);
    fillingStruct(output.m_stS);
    fillingStruct(output.m_mpt);
}

template<>
void fillingStruct(interface_for_test::DForAllModesTests<>& output)
{
    fillingStruct(output.m_saDs);
    output.m_saDs.m_i -= 2;                 // == SimplyAssignable::m_i - 2;

    fillingStruct(output.m_diamond);
    fillingStruct(output.m_sptCs);
    fillingStruct(output.m_saaToStS);
    fillingStruct(output.m_saStS);
    fillingStruct(output.m_stS);
    fillingStruct(output.m_mpt);
}

template<>
void fillingStruct(descendant_interface::SimpleStruct<>& output)
{
    output.m_i = 218418478;
}

template<>
void fillingStruct(descendant_interface::DiamondDescendant<>& output)
{
    fillingStruct(static_cast<interface_for_test::Diamond<>&>(output));
    fillingStruct(output.m_sSt);
    output.m_ldouble = 0.982345792387523523l;
}

template<>
void fillingStruct(another_yet_interface::SimpleStruct<>& output)
{
    output.m_i = 27838;
}

template<>
void fillingStruct(interface_for_test::SimplyAssignableWithoutSerializationFunctions<>& output)
{
    output.m_i = 24801;
}

template<>
void fillingStruct(restricted_structs::SimplyAssignableWithoutSerializationFunctions& output)
{
    output.i = 104;
}

template<>
void fillingStruct(interface_for_test::ContainSimplyAssignableWithoutSerializationFunctions<>& output)
{
    fillingStruct(output.m_sawsf);
    fillingStruct(output.m_npfSawsf);
}

template<>
void fillingStruct(std::string& output)
{
    output = "asdfg";
}

template<>
void fillingStruct(std::wstring& output)
{
    output = L"asdfg";
}

template<>
void fillingStruct(with_std_included_interface::OneBigType<>& output)
{
    fillingStruct(output.m_string1);
    fillingStruct(output.m_string2);
    output.m_vector1.push_back(output.m_string1 + "123");
    output.m_vector1.push_back(output.m_string1 + "456");
    output.m_vector1.push_back(output.m_string1 + "789");
    output.m_vector2.push_back(5);
    output.m_vector2.push_back(12);
    output.m_vector2.push_back(35456453);
    output.m_map1.emplace(std::make_pair(123, .324f));
    output.m_map1.emplace(std::make_pair(17, 65468.324f));
    output.m_map2.emplace(std::make_pair("wete", std::vector<uint8_t>{54, 124, 21, 19}));
    output.m_map2.emplace(std::make_pair("wtassac", std::vector<uint8_t>{23, 2, 0, 139}));
    output.m_map2.emplace(std::make_pair(" b34b", std::vector<uint8_t>{2, 78, 235, 16}));
    output.m_tuple1 = { 93, 3209857239, "099234" };
    output.m_tuple2 = { 35232632.2 };
}

} // namespace ft_helpers
