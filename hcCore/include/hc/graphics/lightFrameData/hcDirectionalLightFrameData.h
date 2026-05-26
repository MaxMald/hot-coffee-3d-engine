#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief GPU-aligned data for a single directional light, intended to be
   * uploaded to the GPU once per frame as part of a light buffer
   * (UBO or SSBO).
   *
   * The struct is aligned to 16 bytes (alignas(16)) to satisfy
   * std140/std430 layout requirements.
   */
  struct alignas(16) HC_CORE_EXPORT DirectionalLightFrameData
  {
    Vector4f directionAndIntensity; ///< Normalized light direction. W component stores intensity.
    Color    color;                 ///< Linear RGB emission color.
  };

  static_assert(sizeof(DirectionalLightFrameData) == 32,
    "DirectionalLightFrameData size mismatch.");
  static_assert(offsetof(DirectionalLightFrameData, directionAndIntensity) == 0,
    "DirectionalLightFrameData::directionAndIntensity offset mismatch.");
  static_assert(offsetof(DirectionalLightFrameData, color) == 16,
    "DirectionalLightFrameData::color offset mismatch.");
}
