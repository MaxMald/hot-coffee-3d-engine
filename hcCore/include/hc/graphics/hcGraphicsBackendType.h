#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace graphicsBackendType
  {
    enum Type : UInt8
    {
      OPENGL = 0
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }
}
