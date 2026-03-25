#pragma once

#include "hc/scene/camera/hcICameraProjection.h"

namespace hc
{
  /**
   * @brief Implements a perspective projection for a camera.
   */
  class HC_CORE_EXPORT PerspectiveCameraProjection :
    public ICameraProjection,
    public ISerializable
  {
  public:
    PerspectiveCameraProjection();
    ~PerspectiveCameraProjection() override;

    /**
     * @copydoc ICameraProjection::getType
     *
     * @return The projection type (perspective).
     */
    projectionType::Type getType() const override;

    /**
     * @copydoc ICameraProjection::getProjectionMatrix
     *
     * @return The perspective projection matrix based on the current parameters.
     */
    Matrix4 getProjectionMatrix() override;

    /**
     * @copydoc ICameraProjection::update
     *
     * Updates the cached projection matrix if any parameters have changed.
     */
    void update() override;

    void serialize(BinaryWriter& writer) const override;

    void deserialize(BinaryReader& reader) override;

    /**
     * @brief Sets the vertical field of view angle (in radians or degrees,
     * depending on Angle type).
     * 
     * @param fovY The new vertical field of view.
     */
    void setFovY(Angle fovY);

    /**
     * @brief Gets the vertical field of view angle.
     * 
     * @return The current vertical field of view.
     */
    const Angle& getFovY() const;

    /**
     * @brief Sets the aspect ratio (width / height).
     * 
     * @param aspectRatio The new aspect ratio.
     */
    void setAspectRatio(float aspectRatio);

    /**
     * @brief Gets the aspect ratio (width / height).
     * 
     * @return The current aspect ratio.
     */
    float getAspectRatio() const;

    /**
     * @brief Sets the near clipping plane distance.
     * 
     * @param near The new near plane value.
     */
    void setNear(float near);

    /**
     * @brief Gets the near clipping plane distance.
     * 
     * @return The current near plane value.
     */
    float getNear() const;

    /**
     * @brief Sets the far clipping plane distance.
     * 
     * @param far The new far plane value.
     */
    void setFar(float far);

    /**
     * @brief Gets the far clipping plane distance.
     * 
     * @return The current far plane value.
     */
    float getFar() const;

  private:
    Angle m_fovY;
    float m_aspectRatio;
    float m_near;
    float m_far;
    bool m_isDirty;
    Matrix4 m_cachedProjectionMatrix;
  };
}
