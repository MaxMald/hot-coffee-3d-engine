#pragma once

#include "hc/hcMatrix4.h"
#include "hc/hcVector3.h"

namespace hc
{
  /**
   * @brief Represents a 3D transformation (position, rotation, scale).
   */
  class HC_UTILITY_EXPORT Transform
  {
  public:
    Transform();
    virtual ~Transform();

    /**
     * @brief Sets the scale factors.
     * 
     * @param scaleFactors The new scale as a Vector3f.
     */
    void setScale(const Vector3f& scaleFactors);

    /**
     * @brief Sets the scale factors.
     * 
     * @param x Scale along the X axis.
     * @param y Scale along the Y axis.
     * @param z Scale along the Z axis.
     */
    void setScale(float x, float y, float z);

    /**
     * @brief Gets the current scale factors.
     * 
     * @return The scale as a Vector3f.
     */
    const Vector3f& getScale() const;

    /**
     * @brief Sets the position.
     * 
     * @param position The new position as a Vector3f.
     */
    void setPosition(const Vector3f& position);

    /**
     * @brief Sets the position.
     * 
     * @param x Position along the X axis.
     * @param y Position along the Y axis.
     * @param z Position along the Z axis.
     */
    void setPosition(float x, float y, float z);

    /**
     * @brief Gets the current position.
     * 
     * @return The position as a Vector3f.
     */
    const Vector3f& getPosition() const;

    /**
     * @brief Sets the rotation using Euler angles (in radians).
     *
     * @param eulerAngles The new rotation as a Vector3f (pitch, yaw, roll).
     */
    void setRotation(const Vector3f& eulerAngles);

    /**
     * @brief Sets the rotation using Euler angles (in radians).
     * 
     * @param x Rotation around the X axis (pitch).
     * @param y Rotation around the Y axis (yaw).
     * @param z Rotation around the Z axis (roll).
     */
    void setRotation(float x, float y, float z);

    /**
     * @brief Gets the current rotation as Euler angles (in radians).
     * 
     * @return The rotation as a Vector3f (pitch, yaw, roll).
     */
    const Vector3f& getRotation() const;

    /**
     * @brief Computes the transformation matrix for this transform.
     * @return The combined transformation as a Matrix4.
     */
    Matrix4 getMatrix() const;

  private:
    Vector3f m_position;
    Vector3f m_scale;
    Vector3f m_rotation;
  };
}
