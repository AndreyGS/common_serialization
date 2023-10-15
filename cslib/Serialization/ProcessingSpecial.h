/**
 * @file ProcessingSpecial.h
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

namespace common_serialization
{

namespace csp
{

namespace processing
{

template<typename T, typename A, serialization_concepts::ISerializationCapableContainer S, serialization_concepts::IPointersMap PM>
Status serializeData(const Vector<T, A>& value, context::Data<S, PM>& context)
{
    RUN(serializeDataHelper(value.size(), context));
    RUN(serializeDataHelper(value.data(), value.size(), context));
    
    return Status::kNoError;
}

template<typename T, typename A, serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::IPointersMap PM>
Status deserializeData(context::Data<D, PM>& input, Vector<T, A>& value)
{
    value.clear();

    typename Vector<T, A>::size_type size = 0;
    RUN(deserializeDataHelper(input, size));
    RUN(value.reserve(size));
    RUN(deserializeDataHelper(input, size, value.data()));
    value.m_dataSize = size;

    return Status::kNoError;
}

} // namespace processing

} // namespace csp

} // namespace common_serialization
