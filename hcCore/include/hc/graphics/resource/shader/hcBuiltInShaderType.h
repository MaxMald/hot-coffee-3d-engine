#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/shader/hcShaderStageType.h"

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
      SkyboxVertex,
      SkyboxFragment,
      FinalPassFragment,
      ShadowMapVertex,
      ShadowMapFragment,
      Count
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
    HC_CORE_EXPORT shaderStageType::Type GetShaderStageType(Type type);
  }
}
