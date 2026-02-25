#include "hc/scene/camera/hcCamera.h"

namespace hc
{
  Camera::Camera() :
    m_position(0.0f, 0.0f, 0.0f),
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_projectionType(projectionType::Perspective)
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
    Vector3f right = getRight();
    m_up = right.cross(m_direction).normalized();
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

  Vector3f Camera::getRight() const
  {
    return m_direction.cross(m_up).normalized();
  }

  void Camera::move(const Vector3f& delta)
  {
    m_position += delta;
  }

  void Camera::lookAt(const Vector3f& target)
  {
    m_direction = (target - m_position).normalized();
    Vector3f right = getRight();
    m_up = right.cross(m_direction).normalized();
  }

  void Camera::rotate(const Vector3f& eulerAngles)
  {
    Matrix4 rotationMatrix = Matrix4::Rotation(eulerAngles);
    m_direction = (rotationMatrix * Vector4f(m_direction, 0.0f)).xyz().normalized();
    Vector3f right = getRight();
    m_up = right.cross(m_direction).normalized();
  }

  void Camera::setProjectionType(projectionType::Type type)
  {
    m_projectionType = type;
  }

  projectionType::Type Camera::getProjection() const
  {
    return m_projectionType;
  }

  Matrix4 Camera::getViewMatrix() const
  {
    return Matrix4::LookAt(m_position, m_position + m_direction, m_up);
  }

  Matrix4 Camera::getProjectionMatrix()
  {
    if (m_projectionType == projectionType::Perspective)
    {
      return m_perspectiveProjection.getProjectionMatrix();
    }
    else // Orthographic
    {
      return m_orthographicProjection.getProjectionMatrix();
    }
  }

  ICameraProjection* Camera::getCameraProjection()
  {
    if (m_projectionType == projectionType::Perspective)
    {
      return &m_perspectiveProjection;
    }
    else // Orthographic
    {
      return &m_orthographicProjection;
    }
  }
}
