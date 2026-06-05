#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace componentType
  {
    enum Type : UInt16
    {
      Mesh = 0,
      //Light = 1,
      Camera = 2,
      OmniLight = 3,
      SpotLight = 4,
      DirectionalLight = 5,
      Unknown = 65535
    };

    Type HC_CORE_EXPORT FromString(const String& str);
    String HC_CORE_EXPORT ToString(Type type);
  }
}
