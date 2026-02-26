#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace primitiveShapeType
  {
    enum Type : UInt8
    {
      Cube = 0,
      Cone,
      Count
    };

    HC_CORE_EXPORT String toString(Type type);
    HC_CORE_EXPORT Type fromString(const String& str);
  }
}
