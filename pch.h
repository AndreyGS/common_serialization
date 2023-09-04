#pragma once


#ifdef USER_MODE // must be defined when c++ standard library is availible
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <concepts>
#include <type_traits>
#include <limits>

#include "allocators/PlatformDependent/UserModeMemoryManagement.h"

#else // !USER_MODE

#include "std_equivalents/std_equivalents.h"
#include "std_equivalents/new.h"

#ifdef LINUX_KERNEL

#include "allocators/PlatformDependent/LinuxKernelMemoryManagement.h"

#elif defined WINDOWS_KERNEL

#include "allocators/PlatformDependent/WindowsKernelMemoryManagement.h"

#endif // LINUX_KERNEL || WINDOWS_KERNEL

#endif // USER_MODE

#include "Allocators/AllocatorConcepts.h"
