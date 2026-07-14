#pragma once

#include "hc/graphics/lightShadowFrameData/hcDirectionalLightShadowFrameData.h"

namespace hc
{
  /**
   * @brief Aggregated light shadow data for a single frame, containing arrays of
   * directional light shadow data. This struct is intended to be
   * uploaded to the GPU once per frame as part of a light shadow buffer.
   */
  struct alignas(16) HC_CORE_EXPORT LightShadowFrameData
  {
    static constexpr UInt32 MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA = 4;

    DirectionalLightShadowFrameData directionalLightShadowData[MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA];
  };

  static_assert(sizeof(LightShadowFrameData) % 16 == 0, "LightShadowFrameData must be 16-byte aligned");
}
