#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/lightFrameData/hcDirectionalLightFrameData.h"
#include "hc/graphics/lightFrameData/hcOmniLightFrameData.h"
#include "hc/graphics/lightFrameData/hcSpotLightFrameData.h"

namespace hc
{
  /**
   * @brief Aggregated light data for a single frame, containing arrays of
   * directional, omni, and spot lights. This struct is intended to be
   * uploaded to the GPU once per frame as part of a light buffer.
   */
  struct alignas(16) HC_CORE_EXPORT LightFrameData
  {
    static constexpr UINT32 MAX_OMNI_LIGHTS = 16;
    static constexpr UINT32 MAX_SPOT_LIGHTS = 8;
    static constexpr UINT32 MAX_DIRECTIONAL_LIGHTS = 4;

    DirectionalLightFrameData directionalLights[MAX_DIRECTIONAL_LIGHTS];
    OmniLightFrameData omniLights[MAX_OMNI_LIGHTS];
    SpotLightFrameData spotLights[MAX_SPOT_LIGHTS];

    Int32 numSpotLights = 0;
    Int32 numOmniLights = 0;
    Int32 numDirectionalLights = 0;
  };
}
