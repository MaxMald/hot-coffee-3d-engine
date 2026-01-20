#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace lightType
  {
    enum Type : UInt8
    {
      Directional,
      Point,
      Spot
    };

    /**
     * @brief Converts a LightType to its string representation.
     */
    String HC_CORE_EXPORT toString(Type type);
  }
}
