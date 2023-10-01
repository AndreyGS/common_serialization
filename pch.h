#pragma once

#ifdef USER_MODE // must be defined when c++ standard library is availible
#include <cstdint>
#endif // USER_MODE

#ifdef USER_MODE // must be defined when c++ standard library is availible
#include <cstdlib>
#include <cassert>
#include <concepts>
#include <type_traits>
#include <limits>
#include <unordered_map>

#include "allocators/PlatformDependent/UserModeMemoryManagement.h"

#else // !USER_MODE

#include "std_equivalents/std_equivalents.h"

#ifdef LINUX_KERNEL

#include "allocators/PlatformDependent/LinuxKernelMemoryManagement.h"

#elif defined WINDOWS_KERNEL

#include "allocators/PlatformDependent/WindowsKernelMemoryManagement.h"

#endif // LINUX_KERNEL || WINDOWS_KERNEL

#include "std_equivalents/new.h"

#endif // USER_MODE

#include "helpers.h"

#include "Allocators/AllocatorConcepts.h"
