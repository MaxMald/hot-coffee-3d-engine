#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcProjectionType.h"
#include "hc/hcPerspectiveCameraProjection.h"
#include "hc/hcOrthographicCameraProjection.h"

namespace hc
{
  class CameraManager;

  /**
   * @brief Represents a camera in the engine.
   */
  class HC_CORE_EXPORT Camera
  {
  public:
    ~Camera();

    /**
     * @brief Sets the position of the camera.
     *
     * @param position The new position.
     */
    void setPosition(const Vector3f& position);

    /**
     * @brief Gets the position of the camera.
     *
     * @return The current position.
     */
    const Vector3f& getPosition() const;

    /**
     * @brief Sets the direction the camera is looking at.
     *
     * @param direction The new direction vector.
     */
    void setDirection(const Vector3f& direction);

    /**
     * @brief Gets the direction the camera is looking at.
     *
     * @return The current direction vector.
     */
    const Vector3f& getDirection() const;

    /**
     * @brief Sets the up vector of the camera.
     *
     * @param up The new up vector.
     */
    void setUp(const Vector3f& up);

    /**
     * @brief Gets the up vector of the camera.
     *
     * @return The current up vector.
     */
    const Vector3f& getUp() const;

    /**
     * @brief Sets the projection type of the camera.
     * 
     * @param type The new projection type.
     */
    void setProjection(projectionType::Type type);

    /*
    * @brief Gets the projection type of the camera.
    * 
    * @returns The current projection type.
    */
    projectionType::Type getProjection() const;

    /**
     * @brief Computes and returns the view matrix of the camera.
     *
     * @return The view matrix.
     */
    Matrix4 getViewMatrix() const;

    /**
     * @brief Computes and returns the projection matrix of the camera.
     * 
     * @return The projection matrix.
     */
    Matrix4 getProjectionMatrix();

    /**
     * @brief Gets the active camera projection implementation.
     * 
     * @return Pointer to the active ICameraProjection.
     */
    ICameraProjection* getActiveProjection();

  private:
    Vector3f m_position;
    Vector3f m_direction;
    Vector3f m_up;
    projectionType::Type m_projectionType;
    PerspectiveCameraProjection m_perspectiveProjection;
    OrthographicCameraProjection m_orthographicProjection;

    Camera();

    friend class CameraManager;
  };
}
