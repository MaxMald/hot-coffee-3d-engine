#pragma once

#include "hc/graphics/hcCameraMatrices.h"

namespace hc
{
  /**
   * @brief Contains transformation data required for rendering an object.
   */
  struct HC_CORE_EXPORT RenderContext
  {
    /**
     * @brief The camera's projection and view matrices.
     */
    CameraMatrices cameraMatrices;

    /**
     * @brief The position of the camera in world space.
     */
    Vector3f cameraPosition;

    /**
     * @brief The transform matrix for the rendered object.
     */
    Matrix4 transform = Matrix4::Identity();

    /**
     * @brief The position of the model in world space, extracted from the transform.
     */
    Vector3f modelPosition;
  };
}
