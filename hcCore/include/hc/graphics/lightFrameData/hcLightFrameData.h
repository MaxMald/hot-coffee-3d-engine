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
  struct HC_CORE_EXPORT LightFrameData
  {
    Vector<DirectionalLightFrameData> directionalLights;
    Vector<OmniLightFrameData> omniLights;
    Vector<SpotLightFrameData> spotLights;
  };
}
