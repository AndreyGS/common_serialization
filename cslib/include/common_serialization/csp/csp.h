/**
 * @file cslib/include/common_serialization/csp/main.h
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

#include "common_serialization/csp/Concepts.h"
#include "common_serialization/csp/Traits.h"
#include "common_serialization/csp/ISerializable.h"
#include "common_serialization/csp/Macros.h"
#include "common_serialization/csp/context/Common.h"
#include "common_serialization/csp/context/CommonFlags.h"
#include "common_serialization/csp/context/Data.h"
#include "common_serialization/csp/context/DataFlags.h"
#include "common_serialization/csp/context/Message.h"
#include "common_serialization/csp/messaging/Client.h"
#include "common_serialization/csp/messaging/IClientDataHandlerTraits.h"
#include "common_serialization/csp/messaging/GenericServerDataHandlerRegistrar.h"
#include "common_serialization/csp/messaging/IClientSpeaker.h"
#include "common_serialization/csp/messaging/IServerDataHandler.h"
#include "common_serialization/csp/messaging/IServerDataHandlerBase.h"
#include "common_serialization/csp/messaging/IServerDataHandlerRegistrar.h"
#include "common_serialization/csp/messaging/IServerDataHandlerTraits.h"
#include "common_serialization/csp/messaging/Server.h"
#include "common_serialization/csp/messaging/service_structs/Interface.h"
#include "common_serialization/csp/processing/Contexts.h"
#include "common_serialization/csp/processing/DataBodyProcessor.h"
#include "common_serialization/csp/processing/DataVersionConverters.h"
#include "common_serialization/csp/processing/Rw.h"
#include "common_serialization/csp/processing/Status.h"
