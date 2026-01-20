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

    String toString(Type type);

    Type fromString(const String& str);
  }
}
