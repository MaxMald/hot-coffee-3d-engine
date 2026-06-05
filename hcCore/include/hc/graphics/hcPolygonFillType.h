#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace polygonFillType
  {
    enum Type : UInt8
    {
      Undefined = 0,
      Solid,
      Wireframe,
      Point
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }
}
