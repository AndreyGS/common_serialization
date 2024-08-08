/**
 * @file UnitTests/for_tests_helpers/include/ft_helpers/CleanAfterStruct.h
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

namespace ft_helpers
{

// it shall be used only for input structs, that are filled with allocating heap memory
// pointers of output structs are cleaned up with context::DeserializeExtendedPointersProcessing destructor
template<typename T>
void cleanAfterStruct(T& output)
{ }

template<>
void cleanAfterStruct(interface_for_test::SpecialProcessingType<>& output);
template<>
void cleanAfterStruct(interface_for_test::ManyPointersType<>& output);
template<>
void cleanAfterStruct(interface_for_test::SForAllModesTests_Version0<>& output);
template<>
void cleanAfterStruct(interface_for_test::SForAllModesTests_Version2<>& output);
template<>
void cleanAfterStruct(interface_for_test::DForAllModesTests<>& output);

} // namespace ft_helpers
