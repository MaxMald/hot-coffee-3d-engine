#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace materialRenderMode
  {
    /**
     * @brief Enumeration of material render modes.
     *
     * Defines the rendering mode for materials, which can affect how they are
     * rendered in the graphics pipeline.
     */
    enum class Type : UInt8
    {
      Background = 0,
      Opaque,
      Transparent
    };

    String HC_CORE_EXPORT toString(Type renderMode);
    Type HC_CORE_EXPORT fromString(const String& str);
  }
}
