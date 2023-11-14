/**
 * @file SpecialTypesCleanAfterStruct.cpp
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

#include "SpecialTypesCleanAfterStruct.h"

namespace special_types
{


template<>
void cleanAfterStruct(ManyPointersTypeSerializable<>& output)
{
    delete[] output.getPInt();
}

template<>
void cleanAfterStruct(ForAllFlagsTests1_Version0<>& output)
{
    cleanAfterStruct(output.getSaDs());
    cleanAfterStruct(output.getDiamond());
    cleanAfterStruct(output.getSptCs());
    cleanAfterStruct(output.getSaaToStS());
    cleanAfterStruct(output.getSaStS());
    cleanAfterStruct(output.getStS());
    cleanAfterStruct(output.getMpT());
}

template<>
void cleanAfterStruct(ForAllFlagsTests1_Version1<>& output)
{
    cleanAfterStruct(output.getSaS());
    cleanAfterStruct(output.getDiamond());
    cleanAfterStruct(output.getSptCs());
    cleanAfterStruct(output.getSaaToStS());
    cleanAfterStruct(output.getSaStS());
    cleanAfterStruct(output.getStS());
    cleanAfterStruct(output.getMpT());
}

template<>
void cleanAfterStruct(ForAllFlagsTests1<>& output)
{
    cleanAfterStruct(output.getSaDs());
    cleanAfterStruct(output.getDiamond());
    cleanAfterStruct(output.getSptCs());
    cleanAfterStruct(output.getSaaToStS());
    cleanAfterStruct(output.getSaStS());
    cleanAfterStruct(output.getStS());
    cleanAfterStruct(output.getMpT());
}

} // namespace special_types