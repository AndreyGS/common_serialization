/**
 * @file cslib/include/common_serialization/csp/messaging/IDataHandlerBase.h
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
#include "common_serialization/csp/processing/Status.h"

namespace common_serialization::csp::messaging
{

// Do not be confused by it naming
// It is a server that process data messages of CSP

/// @brief Base of common interface of CSP data servers
/// @remark Do not be confused by it naming - it is a server 
///     that process data messages of CSP - not database server 
class IDataHandlerBase
{
public:
    [[nodiscard]] virtual interface_version_t getMinimumInterfaceVersion() = 0;

    virtual Status handleDataCommon(context::DData<>& ctx, const GenericPointerKeeper& clientId, BinVector& binOutput) = 0;

protected:
    virtual ~IDataHandlerBase() = default;
};

} // namespace common_serialization::csp::messaging
