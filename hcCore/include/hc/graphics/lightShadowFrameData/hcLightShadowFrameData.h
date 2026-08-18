#pragma once

#include "hc/graphics/lightShadowFrameData/hcDirectionalLightShadowFrameData.h"
#include "hc/graphics/lightShadowFrameData/hcSpotLightShadowFrameData.h"

namespace hc
{
  /**
   * @brief Aggregated light shadow data for a single frame, containing arrays of
   * directional and spot light shadow data. This struct is intended to be uploaded to the
   * GPU once per frame as part of a light shadow buffer.
   */
  struct alignas(16) HC_CORE_EXPORT LightShadowFrameData
  {
    static constexpr UInt32 MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA = 4;
    static constexpr UInt32 MAX_SPOT_LIGHTS_SHADOW_DATA = 8;

    DirectionalLightShadowFrameData directionalLightShadowData[MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA];
    SpotLightShadowFrameData spotLightShadowData[MAX_SPOT_LIGHTS_SHADOW_DATA];
  };

  static_assert(sizeof(LightShadowFrameData) % 16 == 0, "LightShadowFrameData must be 16-byte aligned");
}
