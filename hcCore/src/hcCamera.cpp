#include "hc/hcCamera.h"

namespace hc
{
  Camera::Camera() :
    m_position(0.0f, 0.0f, 0.0f),
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_projectionType(projectionType::Perspective),
    m_near(0.1f),
    m_far(1000.0f)
  {
  }

  Camera::~Camera()
  {
  }

  void Camera::setPosition(const Vector3f& position)
  {
    m_position = position;
  }

  const Vector3f& Camera::getPosition() const
  {
    return m_position;
  }

  void Camera::setDirection(const Vector3f& direction)
  {
    m_direction = direction;
  }

  const Vector3f& Camera::getDirection() const
  {
    return m_direction;
  }

  void Camera::setUp(const Vector3f& up)
  {
    m_up = up;
  }

  const Vector3f& Camera::getUp() const
  {
    return m_up;
  }

  void Camera::setProjection(projectionType::Type type)
  {
    m_projectionType = type;
  }

  projectionType::Type Camera::getProjection() const
  {
    return m_projectionType;
  }

  void Camera::setNear(float nearPlane)
  {
    m_near = nearPlane;
  }

  float Camera::getNear() const
  {
    return m_near;
  }

  void Camera::setFar(float farPlane)
  {
    m_far = farPlane;
  }

  float Camera::getFar() const
  {
    return m_far;
  }

  Matrix4 Camera::getViewMatrix() const
  {
    return Matrix4::LookAt(m_position, m_position + m_direction, m_up);
  }

  Matrix4 Camera::getProjectionMatrix() const
  {
    // TODO Create the projection matrix based on the projection type
  }
}
