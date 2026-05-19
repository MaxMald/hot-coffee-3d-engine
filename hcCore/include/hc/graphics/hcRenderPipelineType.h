#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace renderPipelineType
  {
    enum Type : UInt8
    {
      Forward = 0,
      DeferredHybrid = 1,
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }
}
