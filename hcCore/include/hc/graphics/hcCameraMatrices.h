#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Camera;

  /**
   * @brief Struct to hold render data for a camera, including projection and view
   * matrices, as well as the camera's world position. The struct is aligned to 16 bytes
   * (alignas(16)) to satisfy std140/std430 layout requirements for GPU buffers.
   */
  struct alignas(16) HC_CORE_EXPORT CameraRenderData
  {
    /**
     * @brief Creates a CameraRenderData instance from the given camera.
     *
     * @param camera The camera to extract data from.
     * 
     * @return A CameraRenderData instance based on the given camera.
     */
    static CameraRenderData Create(Camera& camera);

    /**
     * @brief Creates a CameraRenderData instance from the given camera (const version).
     * This version uses the cached projection matrix. Make sure to update the camera
     * before calling this if you want to ensure the projection matrix is up to date.
     *
     * @param camera The camera to extract data from.
     *
     * @return A CameraRenderData instance based on the given camera.
     */
    static CameraRenderData Create(const Camera& camera);

    Matrix4   projectionMatrix = Matrix4::Identity();           ///< Projection matrix transforming camera space to clip space.
    Matrix4   viewMatrix = Matrix4::Identity();                 ///< View matrix transforming world space to camera space.
    Vector3f  cameraWorldPosition = Vector3f(0.0f, 0.0f, 0.0f); ///< World-space position of the camera.
    float     padding0 = 0.0f;
  };

  static_assert(sizeof(CameraRenderData) % 16 == 0, "CameraRenderData must be 16-byte aligned");
}
