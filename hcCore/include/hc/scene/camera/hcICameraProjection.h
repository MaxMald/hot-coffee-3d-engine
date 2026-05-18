#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/camera/hcProjectionType.h"

namespace hc
{
  /**
   * @brief Interface for camera projection strategies.
   *
   * Provides an abstraction for different camera projection types (e.g., perspective, orthographic).
   * Implementations must provide the projection type and the corresponding
   * projection matrix.
   */
  class HC_CORE_EXPORT ICameraProjection
  {
  public:
    virtual ~ICameraProjection() = default;

    /**
     * @brief Gets the type of the projection (e.g., perspective, orthographic).
     * 
     * @return The projection type.
     */
    virtual projectionType::Type getType() const = 0;

    /**
     * @brief Computes and returns the projection matrix for the camera.
     * 
     * @return The projection matrix.
     */
    virtual Matrix4 getProjectionMatrix() = 0;

    /**
     * @brief Gets the cached projection matrix without recomputing it. This should be
     * used when the projection parameters have not changed since the last update, to
     * avoid unnecessary calculations.
     *
     * @return The cached projection matrix.
     */
    virtual Matrix4 getCachedProjectionMatrix() const = 0;

    /**
     * @brief Updates the projection parameters if they have changed.
     */
    virtual void update() = 0;

  protected:
    ICameraProjection() = default;
  };
}
