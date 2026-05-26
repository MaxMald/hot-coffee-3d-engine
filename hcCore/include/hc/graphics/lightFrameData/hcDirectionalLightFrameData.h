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
    Vector4f direction;        ///< Normalized light direction (w unused).
    Color    color;            ///< Linear RGB emission color.
    float    intensity = 0.0f; ///< Scalar multiplier applied to color.
    float    padding0 = 0.0f;
    float    padding1 = 0.0f;
    float    padding2 = 0.0f;
  };

  static_assert(sizeof(DirectionalLightFrameData) == 48,
    "DirectionalLightFrameData size mismatch.");
  static_assert(offsetof(DirectionalLightFrameData, direction) == 0,
    "DirectionalLightFrameData::direction offset mismatch.");
  static_assert(offsetof(DirectionalLightFrameData, color) == 16,
    "DirectionalLightFrameData::color offset mismatch.");
  static_assert(offsetof(DirectionalLightFrameData, intensity) == 32,
    "DirectionalLightFrameData::intensity offset mismatch.");
  static_assert(offsetof(DirectionalLightFrameData, padding0) == 36,
    "DirectionalLightFrameData::padding0 offset mismatch.");
  static_assert(offsetof(DirectionalLightFrameData, padding1) == 40,
    "DirectionalLightFrameData::padding1 offset mismatch.");
  static_assert(offsetof(DirectionalLightFrameData, padding2) == 44,
    "DirectionalLightFrameData::padding2 offset mismatch.");
}
