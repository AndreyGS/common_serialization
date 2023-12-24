/**
 * @file cslib/include/common_serialization/csp/messaging/IDataServerBase.h
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

#include "common_serialization/Containers/Walker.h"
#include "common_serialization/csp/messaging/DataServersKeeper.h"
#include "common_serialization/csp/processing/Statuses.h"

namespace common_serialization::csp::messaging
{

// Do not be confused by it naming
// It is a server that process data messages of CSP
class IDataServerBase
{
public:
    virtual Status handleDataConcrete(context::DInOutData<>& ctx, BinVector& binOutput) = 0;
    virtual [[nodiscard]] interface_version_t getMinimumHandlerSupportedInterfaceVersion() = 0;

    static Status handleDataCommon(context::Common<BinWalker>& ctxCommon, BinVector& binOutput);

protected:
    constexpr IDataServerBase() { }
    constexpr ~IDataServerBase() { }
};

inline Status IDataServerBase::handleDataCommon(context::Common<BinWalker>& ctxCommon, BinVector& binOutput)
{
    context::DInOutData<> ctx(ctxCommon);
    Id id;

    RUN(processing::deserializeInOutDataContext(ctx, id));
    
    context::DataFlags dataFlags = ctx.getDataFlags();

    Vector<GenericPointerKeeper> addedPointers;
    if (dataFlags.allowUnmanagedPointers)
        ctx.setAddedPointers(&addedPointers);

    std::unordered_map<uint64_t, void*> pointersMap;
    if (dataFlags.checkRecursivePointers)
        ctx.setPointersMap(&pointersMap);
    
    IDataServerBase* pServer{ nullptr };
    Status status = Status::kNoError;

    if (Status status = GetDataServersKeeper().findServer(id, pServer); statusSuccess(status))
    {
        status = pServer->handleDataConcrete(ctx, binOutput);
    }
    // If we have more than one DataServer
    else if (status == Status::kErrorMoreEntires)
    {
        Vector<IDataServerBase*, RawStrategicAllocatorHelper<IDataServerBase*>> servers;
        RUN(GetDataServersKeeper().findServers(id, servers));

        status = Status::kNoError;

        for (auto pServer : servers)
            SET_NEW_ERROR(pServer->handleDataConcrete(ctx, binOutput));
    }
    else
        return status;

    if (binOutput.size() == 0 && statusSuccess(status))
        return processing::serializeStatusSuccess(binOutput, ctx.getProtocolVersion(), status);
    else
        return status;
}

} // namespace common_serialization::csp::messaging
