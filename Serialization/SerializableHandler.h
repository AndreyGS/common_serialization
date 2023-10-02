/**
 * @file SerializableHandler.h
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

#include "ISerializable.h"

namespace common_serialization
{

template<typename In, typename Out>
class SerializableHandler
{
public:
    template<serializable_concepts::IDeserializationCapableContainer D, serializable_concepts::ISerializationCapableContainer S>
    Status EnterHandler(D& input, SerializationFlags flags, S& output)
    {
        In inStruct;
        RUN(inStruct.deserializeNext(input, flags));
        Out outStruct;
        RUN(this->Process(inStruct, outStruct));
        
        return outStruct.serializeNext(output, flags);
    }

    template<serializable_concepts::ISerializationCapableContainer S>
    virtual Status Process(const In& inStruct, Out& outStruct) = 0;
};

} // namespace common_serialization