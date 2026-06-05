#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace primitiveModelType
  {
    enum Type : Int8
    {
      Undefined = -1,
      Cube = 0,
      Cone,
      Sphere,
      Cylinder,
      Plane,
      Pyramid,
      ConeNoBase,
      Count
    };

    HC_CORE_EXPORT String toString(Type type);
    HC_CORE_EXPORT Type fromString(const String& str);
  }
}
