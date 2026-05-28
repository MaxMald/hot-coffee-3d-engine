#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief GPU-aligned data for a single omni light, intended to be
   * uploaded to the GPU once per frame as part of a light buffer
   * (UBO or SSBO).
   *
   * The struct is aligned to 16 bytes (alignas(16)) to satisfy
   * std140/std430 layout requirements.
   */
  struct alignas(16) HC_CORE_EXPORT OmniLightFrameData
  {
    Vector4f position;         ///< World-space position (w unused).
    Color    color;            ///< Linear RGB emission color.
    float    range = 0.0f;     ///< Maximum influence radius in world units.
    float    intensity = 0.0f; ///< Scalar multiplier applied to color.
    float    padding0 = 0.0f;
    float    padding1 = 0.0f;
  };

  static_assert(sizeof(OmniLightFrameData) == 48,
    "OmniLightFrameData size mismatch.");
  static_assert(offsetof(OmniLightFrameData, position) == 0,
    "OmniLightFrameData::position offset mismatch.");
  static_assert(offsetof(OmniLightFrameData, color) == 16,
    "OmniLightFrameData::color offset mismatch.");
  static_assert(offsetof(OmniLightFrameData, range) == 32,
    "OmniLightFrameData::range offset mismatch.");
  static_assert(offsetof(OmniLightFrameData, intensity) == 36,
    "OmniLightFrameData::intensity offset mismatch.");
  static_assert(offsetof(OmniLightFrameData, padding0) == 40,
    "OmniLightFrameData::padding0 offset mismatch.");
  static_assert(offsetof(OmniLightFrameData, padding1) == 44,
    "OmniLightFrameData::padding1 offset mismatch.");
}
