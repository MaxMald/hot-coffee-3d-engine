#pragma once

#include "hc/scene/camera/hcICameraProjection.h"

namespace hc
{
  /**
   * @brief Implements an orthographic projection for a camera.
   */
  class HC_CORE_EXPORT OrthographicCameraProjection : 
    public ICameraProjection,
    public io::ISerializable
  {
  public:
    OrthographicCameraProjection();
    ~OrthographicCameraProjection() override;

    /**
     * @copydoc ICameraProjection::getType
     *
     * @return The projection type (orthographic).
     */
    projectionType::Type getType() const override;

    /**
     * @copydoc ICameraProjection::getProjectionMatrix
     *
     * @return The orthographic projection matrix based on the current view
     * volume and near/far planes.
     */
    Matrix4 getProjectionMatrix() override;

    /**
     * @copydoc ICameraProjection::getCachedProjectionMatrix
     */
    Matrix4 getCachedProjectionMatrix() const override;

    /**
     * @copydoc ICameraProjection::update
     *
     * Updates the cached projection matrix if any parameters have changed.
     */
    void update() override;

    /**
     * @copydoc ISerializable::serialize
     */
    void serialize(io::BinaryWriter& writer) const override;

    /**
     * @copydoc ISerializable::deserialize
     */
    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Gets the left boundary of the view volume.
     * 
     * @return The left value.
     */
    float getLeft() const;

    /**
     * @brief Sets the left boundary of the view volume.
     * 
     * @param left The new left value.
     */
    void setLeft(float left);

    /**
     * @brief Gets the right boundary of the view volume.
     * 
     * @return The right value.
     */
    float getRight() const;

    /**
     * @brief Sets the right boundary of the view volume.
     * 
     * @param right The new right value.
     */
    void setRight(float right);

    /**
     * @brief Gets the top boundary of the view volume.
     * 
     * @return The top value.
     */
    float getTop() const;

    /**
     * @brief Sets the top boundary of the view volume.
     * 
     * @param top The new top value.
     */
    void setTop(float top);

    /**
     * @brief Gets the bottom boundary of the view volume.
     * 
     * @return The bottom value.
     */
    float getBottom() const;

    /**
     * @brief Sets the bottom boundary of the view volume.
     * 
     * @param bottom The new bottom value.
     */
    void setBottom(float bottom);

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

    /**
     * @brief Sets the aspect ratio (width / height) of the view volume. This is used to
     * automatically adjust the left, right, top, and bottom boundaries based on the
     * current aspect ratio and a fixed vertical size.
     *
     * @param aspectRatio The new aspect ratio.
     */
    void setAspectRatio(float aspectRatio);

  private:
    float m_left;
    float m_right;
    float m_top;
    float m_bottom;
    float m_near;
    float m_far;
    bool m_isDirty;
    Matrix4 m_cachedProjectionMatrix;
  };
}
