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

    static String ToString(Type type);
    static Type FromString(const String& str);
  }
}
