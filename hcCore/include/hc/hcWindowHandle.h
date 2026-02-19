#pragma once

#include "hc/hcCorePrerequisites.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32
#include <windows.h>
struct HWND__;
#endif

namespace hc
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
  using WindowHandle = HWND__*;
#elif HC_PLATFORM == HC_PLATFORM_LINUX
  using WindowHandle = unsigned long;
#else
  using WindowHandle = void*;
#endif
}
