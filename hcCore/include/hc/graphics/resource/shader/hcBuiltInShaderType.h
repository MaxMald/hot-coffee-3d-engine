#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace builtInShaderType
  {
    enum HC_CORE_EXPORT Type : UInt8
    {
      UnlitVertex = 0,
      UnlitFragment,
      LitVertex,
      BlinnPhongForwardFragment,
      BlinnPhongDeferredFragment,
      FullScreenTriangleVertex,
      DeferredLightingFragment,
      Count
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }
}
