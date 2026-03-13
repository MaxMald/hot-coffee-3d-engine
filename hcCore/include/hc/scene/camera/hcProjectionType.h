#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace projectionType
  {
    /**
     * @brief Enumeration of projection types for cameras.
     */
    enum Type : UInt8
    {
      Perspective = 0,
      Orthographic
    };

    Type HC_CORE_EXPORT FromString(const String&);
    String HC_CORE_EXPORT ToString(Type);
  } 
}
