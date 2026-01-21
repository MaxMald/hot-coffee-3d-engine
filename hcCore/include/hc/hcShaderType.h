#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace shaderType
  {
    enum Type : UInt8
    {
      Unlit = 0,
      BlinnPhong = 1,
      Unknown = 255
    };

    HC_CORE_EXPORT String toString(Type type);
    HC_CORE_EXPORT Type fromString(const String& str);
  }
}
