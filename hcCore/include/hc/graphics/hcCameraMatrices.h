#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Struct to hold camera projection and view matrices.
   */
  struct HC_CORE_EXPORT CameraMatrices
  {
    /**
     * @brief The projection matrix.
     */
    Matrix4 projectionMatrix;

    /**
     * @brief The view matrix.
     */
    Matrix4 viewMatrix;
  };
}
