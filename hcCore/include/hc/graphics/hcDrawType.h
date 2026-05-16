#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace drawType
  {
    enum Type
    {
      Triangles,
      Lines,
      LineStrip,
      LineLoop,
      Points
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }
}
