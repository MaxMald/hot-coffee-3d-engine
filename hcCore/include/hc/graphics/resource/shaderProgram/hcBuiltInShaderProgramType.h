#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace builtInShaderProgramType
  {
    enum HC_CORE_EXPORT Type : UInt8
    {
      Unlit = 0,
      BlinnPhongForward,
      BlinnPhongDeferredGeometry,
      DeferredLighting,
      Skybox
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  };
}
