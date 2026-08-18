#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief GPU-aligned data for a single spot light, intended to be
   * uploaded to the GPU once per frame as part of a light buffer
   * (UBO or SSBO).
   *
   * The struct is aligned to 16 bytes (alignas(16)) to satisfy
   * std140/std430 layout requirements.
   */
  struct alignas(16) HC_CORE_EXPORT SpotLightFrameData
  {
    Vector4f position;                ///< World-space position (w unused).
    Vector4f direction;               ///< Normalized cone direction (w unused).
    Color    color;                   ///< Linear RGB emission color.
    float    range = 0.0f;            ///< Maximum influence radius in world units.
    float    innerConeAngle = 0.0f;   ///< Cosine of the inner cone half-angle.
    float    intensity = 0.0f;        ///< Scalar multiplier applied to color.
    float    outerConeAngle = 0.0f;   ///< Cosine of the outer cone half-angle.
    Int32 shadowFrameDataIndex = -1;  ///< Index of the shadow map data for this light, or -1 if shadows are disabled.
    Int32 padding0 = 0;               ///< Padding to ensure 16-byte alignment.
    Int32 padding1 = 0;               ///< Padding to ensure 16-byte alignment.
    Int32 padding2 = 0;               ///< Padding to ensure 16-byte alignment.
  };

  static_assert(sizeof(SpotLightFrameData) % 16 == 0, "SpotLightFrameData size must be a multiple of 16 bytes.");
}
