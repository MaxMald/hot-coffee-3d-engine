#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief GPU-aligned data for a single spot light's shadow metadata, intended to be
   * uploaded to the GPU once per frame as part of a shadow buffer (UBO or SSBO).
   *
   * The struct is aligned to 16 bytes (alignas(16)) to satisfy std140/std430 layout
   * requirements.
   */
  struct alignas(16) HC_CORE_EXPORT SpotLightShadowFrameData
  {
    Matrix4 lightViewProjectionMatrix;  ///< Light's view-projection matrix for shadow mapping.
    float shadowBias = 0.005f;          ///< Bias to reduce shadow acne.
    float shadowStrength = 1.0f;        ///< Strength of the shadow (0.0 to 1.0).
    float projectionNearPlane = 0.1f;   ///< Near plane distance for the light's projection.
    float projectionFarPlane = 100.0f;  ///< Far plane distance for the light's projection.
    Int32 shadowMapIndex = -1;          ///< Index of the shadow map texture in the array.
    Int32 padding0 = 0;                 ///< Padding to ensure 16-byte alignment.
    Int32 padding1 = 0;                 ///< Padding to ensure 16-byte alignment.
    Int32 padding2 = 0;                 ///< Padding to ensure 16-byte alignment.
  };
}

