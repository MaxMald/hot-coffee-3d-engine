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
    Vector4f directionAndIntensity;   ///< Normalized light direction. W component stores intensity.
    Color color;                      ///< Linear RGB emission color.
    Int32 shadowFrameDataIndex = -1;  ///< Index of the shadow map data for this light, or -1 if shadows are disabled.
    Int32 padding1 = 0;               ///< Padding to ensure 16-byte alignment.
    Int32 padding2 = 0;               ///< Padding to ensure 16-byte alignment.
    Int32 padding3 = 0;               ///< Padding to ensure 16-byte alignment.
  };
}
