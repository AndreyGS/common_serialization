/**
 * @file cslib/include/common_serialization/csp/messaging/IDataServerBase.h
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

#include "common_serialization/Containers/Walker.h"
#include "common_serialization/csp/messaging/DataServersKeeper.h"
#include "common_serialization/csp/processing/Status.h"

namespace common_serialization::csp::messaging
{

// Do not be confused by it naming
// It is a server that process data messages of CSP

/// @brief Interface of common CSP data servers
/// @remark Do not be confused by it naming - it is a server 
///     that process data messages of CSP - not database server 
class IDataServerBase
{
public:
    /// @brief Common entry point on data messages handling
    /// @param ctxCommon Deserialized from input common context
    /// @param binOutput Binary data output
    /// @return Status of operation
    static Status handleDataCommon(context::Common<BinWalker>& ctxCommon, const BinVector& clientId, BinVector& binOutput);

    virtual [[nodiscard]] interface_version_t getMinimumInterfaceVersion() = 0;

protected:
    constexpr IDataServerBase() { }
    constexpr ~IDataServerBase() { }

private:
    virtual Status handleDataConcrete(context::DData<>& ctx, const BinVector& clientId, BinVector& binOutput) = 0;
};

inline Status IDataServerBase::handleDataCommon(context::Common<BinWalker>& ctxCommon, const BinVector& clientId, BinVector& binOutput)
{
    context::DData<> ctx(ctxCommon);
    Id id;

    RUN(processing::deserializeDataContext(ctx, id));
    
    context::DataFlags dataFlags = ctx.getDataFlags();

    Vector<GenericPointerKeeper> addedPointers;
    if (dataFlags.allowUnmanagedPointers())
        ctx.setAddedPointers(&addedPointers);

    std::unordered_map<uint64_t, void*> pointersMap;
    if (dataFlags.checkRecursivePointers())
        ctx.setPointersMap(&pointersMap);
    
    IDataServerBase* pServer{ nullptr };
    Status status = Status::kNoError;

    if (Status status = GetDataServersKeeper().findServer(id, pServer); statusSuccess(status))
    {
        status = pServer->handleDataConcrete(ctx, clientId, binOutput);
    }
    // If we have more than one DataServer
    else if (status == Status::kErrorMoreEntires)
    {
        Vector<IDataServerBase*, RawStrategicAllocatorHelper<IDataServerBase*>> servers;
        RUN(GetDataServersKeeper().findServers(id, servers));

        status = Status::kNoError;

        typename BinWalker::size_type bodyPosition = ctx.getBinaryData().tell();

        for (auto pServer : servers)
        {
            context::DData<> ctxTemp(ctx);
            SET_NEW_ERROR(pServer->handleDataConcrete(ctxTemp, clientId, binOutput));

            ctx.getBinaryData().seek(bodyPosition);
        }
    }

    return status;
}

} // namespace common_serialization::csp::messaging
