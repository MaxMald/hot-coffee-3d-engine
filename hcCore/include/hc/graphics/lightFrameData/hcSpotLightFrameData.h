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
    Vector4f position;              ///< World-space position (w unused).
    Vector4f direction;             ///< Normalized cone direction (w unused).
    Color    color;                 ///< Linear RGB emission color.
    float    range = 0.0f;          ///< Maximum influence radius in world units.
    float    innerConeAngle = 0.0f; ///< Cosine of the inner cone half-angle.
    float    intensity = 0.0f;      ///< Scalar multiplier applied to color.
    float    outerConeAngle = 0.0f; ///< Cosine of the outer cone half-angle.
  };

  static_assert(sizeof(SpotLightFrameData) == 64,
    "SpotLightFrameData size mismatch.");
  static_assert(offsetof(SpotLightFrameData, position) == 0,
    "SpotLightFrameData::position offset mismatch.");
  static_assert(offsetof(SpotLightFrameData, direction) == 16,
    "SpotLightFrameData::direction offset mismatch.");
  static_assert(offsetof(SpotLightFrameData, color) == 32,
    "SpotLightFrameData::color offset mismatch.");
  static_assert(offsetof(SpotLightFrameData, range) == 48,
    "SpotLightFrameData::range offset mismatch.");
  static_assert(offsetof(SpotLightFrameData, innerConeAngle) == 52,
    "SpotLightFrameData::innerConeAngle offset mismatch.");
  static_assert(offsetof(SpotLightFrameData, intensity) == 56,
    "SpotLightFrameData::intensity offset mismatch.");
  static_assert(offsetof(SpotLightFrameData, outerConeAngle) == 60,
    "SpotLightFrameData::outerConeAngle offset mismatch.");
}
