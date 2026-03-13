#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace shaderStageType
  {
    enum Type : UInt8
    {
      Vertex = 0,
      Fragment = 1,
      Unknown = 255
    };

    HC_CORE_EXPORT String toString(Type type);
    HC_CORE_EXPORT Type fromString(const String& str);
  }
}
