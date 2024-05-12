/**
 * @file cslib/include/common_serialization/csp/Traits.h
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

namespace common_serialization::csp
{

using Id = Uuid;
using interface_version_t = uint32_t;
using protocol_version_t = uint8_t;
#ifdef X32_PLATFORM
using csp_size_t = uint32_t;
#else
using csp_size_t = uint64_t;
#endif // #ifdef X32_PLATFORM

namespace traits
{

inline constexpr protocol_version_t kProtocolVersions[] = { 1 };
inline constexpr protocol_version_t kProtocolVersionUndefined = 0xff;           // it always must be kind of UINTMAX
inline constexpr interface_version_t kInterfaceVersionUndefined = 0xffffffff;   // it always must be kind of UINTMAX

} // namespace traits

struct Interface
{
    using simply_assignable_fixed_size_tag = std::true_type;

    Id id{ kNullUuid };

    /// @brief The only field that allowed to change since interface publication
    interface_version_t version{ traits::kInterfaceVersionUndefined };

    context::DataFlags mandatoryDataFlags;
    context::DataFlags forbiddenDataFlags;

    [[nodiscard]] constexpr auto operator<=>(const Interface&) const = default;
};

namespace traits
{

constexpr Interface kUndefinedInterface{ kNullUuid, kInterfaceVersionUndefined, {}, {} };

[[nodiscard]] constexpr protocol_version_t getLatestProtocolVersion() noexcept
{
    return kProtocolVersions[0];
}

[[nodiscard]] constexpr bool isProtocolVersionSameAsLatestOur(protocol_version_t foreignProtocolVersion) noexcept
{
    return getLatestProtocolVersion() == foreignProtocolVersion;
}

[[nodiscard]] constexpr protocol_version_t getProtocolVersionsCount() noexcept
{
    return std::size(kProtocolVersions);
}

[[nodiscard]] constexpr bool isProtocolVersionSupported(protocol_version_t foreignProtocolVersion) noexcept
{
    for (protocol_version_t i = 0; i < getProtocolVersionsCount(); ++i)
        if (foreignProtocolVersion == kProtocolVersions[i])
            return true;

    return false;
}

[[nodiscard]] constexpr bool isInterfaceVersionSupported(interface_version_t version, interface_version_t minVersion, interface_version_t maxVersion) noexcept
{
    return maxVersion >= version && version >= minVersion;
}

// Using to find index of version in versions hierarchy of struct to which we must serialize/deserialize
template<typename T>
[[nodiscard]] constexpr interface_version_t getBestCompatInterfaceVersion(interface_version_t compatInterfaceVersion) noexcept
{
    const interface_version_t* pPrivateVersions = T::getPrivateVersions();

    for (interface_version_t i = 0; i < T::getPrivateVersionsCount(); ++i)
        if (pPrivateVersions[i] <= compatInterfaceVersion)
            return pPrivateVersions[i];

    return kInterfaceVersionUndefined;
}

template<typename T>
[[nodiscard]] constexpr interface_version_t getBestSupportedInterfaceVersion(
    interface_version_t minForeignVersion, interface_version_t maxForeignVersion, interface_version_t minCurrentVersion
) noexcept
{
    if (minCurrentVersion > minForeignVersion || minForeignVersion > T::getLatestInterfaceVersion())
        return kInterfaceVersionUndefined;

    if (maxForeignVersion >= T::getLatestInterfaceVersion())
        return T::getLatestInterfaceVersion();
    else
        return maxForeignVersion;
}

template<ISerializationBinContainer _Sbin, ISerializationPointersMap _Spm>
struct SContainersConcrete
{
    using Sbin = _Sbin;
    using Spm = _Spm;
};

using DefaultSContainer = BinVectorT;
using DefaultSpmContainer = std::unordered_map<const void*, csp_size_t>;

using DefaultSContainers = SContainersConcrete<DefaultSContainer, DefaultSpmContainer>;

template<IDeserializationBinContainer _Dbin, IDeserializationPointersMap _Dpm, IGenericPointersKeeperContainer _Gkc>
struct DContainersConcrete
{
    using Dbin = _Dbin;
    using Dpm = _Dpm;
    using Gkc = _Gkc;
};

using DefaultDContainer = BinWalkerT;
using DefaultDpmContainer = std::unordered_map<csp_size_t, void*>;
using DefaultPcContainer = Vector<GenericPointerKeeper>;
template<typename... _Ts>
using DefaultBasicContainer = Vector<_Ts...>;

using DefaultDContainers = DContainersConcrete<DefaultDContainer, DefaultDpmContainer, DefaultPcContainer>;

template<
      ISerializationBinContainer _Sbin
    , ISerializationPointersMap _Spm
    , IDeserializationBinContainer _Dbin
    , IDeserializationPointersMap _Dpm
    , IGenericPointersKeeperContainer _Gkc
    , template<typename...> typename _Bc
>
    requires IBasicContainer<_Bc>
struct SdContainersConcrete
{
    using Sbin = _Sbin;
    using Spm = _Spm;
    using Dbin = _Dbin;
    using Dpm = _Dpm;
    using Gkc = _Gkc;

    using Scs = SContainersConcrete<Sbin, Spm>;
    using Dcs = DContainersConcrete<Dbin, Dpm, Gkc>;

    template<typename... _Ts>
    using Bc = _Bc<_Ts...>;
};

using DefaultSdContainers = SdContainersConcrete<DefaultSContainer, DefaultSpmContainer, DefaultDContainer, DefaultDpmContainer, DefaultPcContainer, DefaultBasicContainer>;

template<bool>
struct _BinContainer
{
    using type = BinVectorT;
};

template<>
struct _BinContainer<true>
{
    using type = BinWalkerT;
};

template<bool _serialize>
using BinContainer = typename _BinContainer<_serialize>::type;

template<bool>
struct _PmContainer
{
    using type = HashMapT<const void*, csp_size_t>;
};

template<>
struct _PmContainer<false>
{
    using type = HashMapT<csp_size_t, void*>;
};

template<bool _serialize>
using PmContainer = typename _PmContainer<_serialize>::type;

template<AnySdContainers _Asdcs, bool>
struct _PcContainer
{
    using type = Dummy;
};

template<AnySdContainers _Asdcs>
struct _PcContainer<_Asdcs, false>
{
    using type = typename _Asdcs::Gkc;
};

template<AnySdContainers _Asdcs, bool B>
using PcContainer = typename _PcContainer<_Asdcs, B>::type;

} // namespace traits

} // namespace common_serialization::csp
