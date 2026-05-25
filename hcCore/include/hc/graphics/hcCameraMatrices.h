#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Camera;

  /**
   * @brief Struct to hold render data for a camera, including projection and view
   * matrices, as well as the camera's world position.
   */
  struct HC_CORE_EXPORT CameraRenderData
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

    /**
     * @brief The projection matrix.
     */
    Matrix4 projectionMatrix = Matrix4::Identity();

    /**
     * @brief The view matrix.
     */
    Matrix4 viewMatrix = Matrix4::Identity();

    /**
     * @brief The camera's world position. This is not strictly necessary for all shaders,
     * but it's often useful to have it available when setting shader uniforms.
     */
    Vector3f cameraWorldPosition = Vector3f(0.0f, 0.0f, 0.0f);
  };
}
