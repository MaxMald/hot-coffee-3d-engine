#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcVector3.h"
#include "hc/hcMatrix4.h"

namespace hc
{
  /**
   * @brief Represents a camera in the engine.
   */
  class HC_CORE_EXPORT Camera
  {
  public:
    Camera();
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
     * @brief Computes and returns the view matrix of the camera.
     * 
     * @return The view matrix.
     */
    Matrix4 getViewMatrix() const;

  private:
    Vector3f m_position;
    Vector3f m_direction;
    Vector3f m_up;
  };
}
