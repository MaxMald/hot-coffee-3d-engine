#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace componentType
  {
    enum Type : UInt8
    {
      Mesh = 0,
      Light,
      Camera,
      Unknown = 255
    };

    Type HC_CORE_EXPORT FromString(const String& str);
    String HC_CORE_EXPORT ToString(Type type);
  }
}
