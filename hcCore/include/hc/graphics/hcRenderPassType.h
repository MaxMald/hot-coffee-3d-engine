#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace renderPassType
  {
    enum Type : UInt8
    {
      Forward = 0,
      DeferredGeometry,
      DeferredLighting,
      Count
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }
}
