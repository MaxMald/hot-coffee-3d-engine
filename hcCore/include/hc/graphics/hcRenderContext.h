#pragma once

#include "hc/graphics/hcPolygonFillType.h"

namespace hc
{
  class Camera;

  /**
   * @brief Contains transformation data required for rendering an object.
   */
  struct HC_CORE_EXPORT RenderContext
  {
    static RenderContext Create(Camera& camera, const Matrix4& modelTransform);

    static RenderContext Create(const Camera& camera, const Matrix4& modelTransform);

    /**
     * @brief The transform matrix for the rendered object.
     */
    Matrix4 transform = Matrix4::Identity();

    /**
     * @brief The position of the camera in world space.
     */
    Vector3f cameraWorldPosition;

    /**
     * @brief The position of the model in world space, extracted from the transform.
     */
    Vector3f modelPosition;

    /**
     * @brief The polygon fill type for rendering the object.
     */
    polygonFillType::Type polygonFillType = polygonFillType::Undefined;
  };
}
