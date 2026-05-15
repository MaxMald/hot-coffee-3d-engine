#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Camera;

  /**
   * @brief Struct to hold camera projection and view matrices.
   */
  struct HC_CORE_EXPORT CameraMatrices
  {
    /**
     * @brief Creates a CameraMatrices instance from the given camera.
     *
     * @param camera The camera to extract matrices from.
     * 
     * @return A CameraMatrices instance containing the projection and view matrices.
     */
    static CameraMatrices Create(Camera& camera);

    /**
     * @brief Creates a CameraMatrices instance from the given camera (const version).
     * This version uses the cached projection matrix. Make sure to update the camera
     * before calling this if you want to ensure the projection matrix is up to date.
     *
     * @param camera The camera to extract matrices from.
     *
     * @return A CameraMatrices instance containing the projection and view matrices.
     */
    static CameraMatrices Create(const Camera& camera);

    /**
     * @brief The projection matrix.
     */
    Matrix4 projectionMatrix = Matrix4::Identity();

    /**
     * @brief The view matrix.
     */
    Matrix4 viewMatrix = Matrix4::Identity();
  };
}
