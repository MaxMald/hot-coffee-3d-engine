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
     * @brief The transform matrix for the rendered object.
     */
    Matrix4 transform;
  };
}
