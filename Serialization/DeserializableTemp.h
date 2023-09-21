/**
 * @file DeserializableTemp.h
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

#include "SerializableTemp.h"


namespace common_serialization
{

template<serializable_concepts::IDeserializationCapableContainer D>
int deserializeThis(D& input, SerT<>& value)
{
    deserializeThis(input, value.i);

    return 0;
}

template<serializable_concepts::IDeserializationCapableContainer D>
int deserializeThis(D& input, SerT2<>& value)
{
    deserializeThis(input, value.k);

    return 0;
}

template<serializable_concepts::IDeserializationCapableContainer D>
int deserializeThis(D& input, SerTInh<>& value)
{
    deserializeThis(input, value.getSerT());
    deserializeThis(input, value.getSerT2());
    deserializeThis(input, value.j);
    deserializeThis(input, value.arr);

    return 0;
}

} // namespace common_serialization
