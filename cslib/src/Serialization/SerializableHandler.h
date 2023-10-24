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

#pragma once

#include "ISerializable.h"
#include "Containers/Walker.h"

#include <unordered_map> // temporary header
#include <semaphore> // temporary header

namespace common_serialization
{

struct SerializableHandlerFlags
{
    uint32_t transactionalMessage               :  1 = 0;
    uint32_t reserved                           : 23 = 0;
    uint32_t doNotUse                           :  8 = 0;   // this bit-field shall have only 24 significant bits
                                                            // 8 bits are using for serialization protocol version
};

// Handler header format
//
//  {
//      uint8_t protocolVersion;
//      SerializableHandlerFlags flags;
//      uint32_t transactionNumber; // optional
//      SerilizableHandlerMessageType messageType;
//  }
//

enum class SerilizableHandlerMessageType : uint_fast32_t
{
    kData = 0x0,                            // message body format:
                                            // 
                                            // {
                                            //     uint8_t serializedData[anysize];
                                            // } 
                                            //
    kStatus = 0x1,                          // message body format:
                                            // 
                                            // {
                                            //     Status lastMessageProcessingStatus;
                                            // }                               
};

class SerializableHandlerBase;

class SerializableHandlersDb
{
public:
    static Status addHandler(uint64_t structNameHash, SerializableHandlerBase* handlerInstance)
    {
        handlersLock.acquire();
        if (!serializableHandlers.contains(structNameHash))
            serializableHandlers[structNameHash] = handlerInstance;
        handlersLock.release();
    }

private:
    static std::unordered_map<uint64_t, SerializableHandlerBase*> serializableHandlers;
    static std::binary_semaphore handlersLock;
};

std::binary_semaphore SerializableHandlersDb::handlersLock{ 1 };

class SerializableHandlerBase
{
public:
    SerializableHandlerBase(uint64_t structNameHash, uint32_t structVersionThis, uint32_t structVersionInterface)
    {
        SerializableHandlersDb::addHandler(structNameHash, this);
    }

    uint32_t getStructVersionThis() { return m_structVersionThis; }
    uint32_t getStructVersionInterface() { return m_structVersionInterface; }

    virtual Status processCommon(uint8_t protocolVersion, Flags flags, uint32_t interfaceVersion, Walker<uint8_t>& input, Vector<uint8_t>& output) = 0;
    /*
    template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::ISerializationCapableContainer S>
    Status EnterHandler(D& input, Flags flags, S& output)
    {
        In inStruct;
        RUN(inStruct.deserializeData(input, flags));
        Out outStruct;
        RUN(this->Process(inStruct, outStruct));
        
        return outStruct.serializeData(output, flags);
    }*/
private:
    uint64_t m_structNameHash{ 0 };
    uint32_t m_structVersionThis{ 0 };
    uint32_t m_structVersionInterface{ 0 };


    //virtual Status Process(const In& inStruct, Out& outStruct) = 0;
};

template<uint32_t MinimumSupportedInterfaceVersion, typename In, typename Out>
class SerializableHandler : public SerializableHandlerBase
{
public:
    SerializableHandler()
        : SerializableHandlerBase(ISerializable<In>::getThisVersion())
    {

    }

    uint64_t getThisVersion() const noexcept override
    {
        return ISerializable<In>::getThisVersion();
    }
    /*
    Status processCommon(uint8_t protocolVersion, Flags flags, uint32_t interfaceVersion, Walker<uint8_t>& input, Vector<uint8_t>& output) override
    {
        output.clear();
        if (MinimumSupportedInterfaceVersion > interfaceVersion)
        {
            output
        }
            return Status::kErrorNotSupportedInterfaceVersion;
        else if (getStructVersionInterface() > interfaceVersion && !flags.interfaceVersionsNotMatch)
        {

        }
        
    }*/
    /*
    template<serialization_concepts::IDeserializationCapableContainer D, serialization_concepts::ISerializationCapableContainer S>
    Status EnterHandler(D& input, Flags flags, S& output)
    {
        In inStruct;
        RUN(inStruct.deserializeData(input, flags));
        Out outStruct;
        RUN(this->Process(inStruct, outStruct));
        
        return outStruct.serializeData(output, flags);
    }

    template<serialization_concepts::ISerializationCapableContainer S>
    virtual Status Process(const In& inStruct, Out& outStruct) = 0;*/
};

} // namespace common_serialization