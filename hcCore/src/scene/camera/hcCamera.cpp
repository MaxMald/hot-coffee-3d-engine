#include "hc/scene/camera/hcCamera.h"

namespace hc
{
  Camera::Camera() :
    m_id(UUID::Generate()),
    m_position(0.0f, 0.0f, 0.0f),
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_projectionType(projectionType::Perspective)
  {
    setAspectRatio(1.77778f); // Default to 16:9 aspect ratio
  }

  Camera::~Camera()
  {
  }

  void Camera::serialize(BinaryWriter& writer) const
  {
    m_id.serialize(writer);
    writer.writeVector3f(m_position);
    writer.writeVector3f(m_direction);
    writer.writeVector3f(m_up);
    writer.writeUInt8(m_projectionType);
    m_orthographicProjection.serialize(writer);
    m_perspectiveProjection.serialize(writer);
  }

  void Camera::deserialize(BinaryReader& reader)
  {
    m_id.deserialize(reader);
    m_position = reader.readVector3f();
    m_direction = reader.readVector3f();
    m_up = reader.readVector3f();
    m_projectionType = static_cast<projectionType::Type>(reader.readUInt8());
    m_orthographicProjection.deserialize(reader);
    m_perspectiveProjection.deserialize(reader);
  }

  const UUID& Camera::getUUID() const
  {
    return m_id;
  }

  void Camera::setAspectRatio(float aspectRatio)
  {
    if (aspectRatio <= 0.0f)
    {
      LogService::Error(
        "Camera: Aspect ratio must be greater than zero. Ignoring setAspectRatio call."
      );
      return;
    }

    m_perspectiveProjection.setAspectRatio(aspectRatio);
    m_orthographicProjection.setAspectRatio(aspectRatio);
  }

  void Camera::setAspectRatio(UInt32 width, UInt32 height)
  {
    if (width == 0 || height == 0)
    {
      LogService::Error(
        "Camera: Viewport size cannot be zero. Ignoring setAspectRatio call."
      );
      return;
    }

    setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
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
    recalculateUp();
  }

  const Vector3f& Camera::getDirection() const
  {
    return m_direction;
  }

  void Camera::roll(const Angle& rollAmount)
  {
    Matrix4 rollRotation = Matrix4::RotationAxis(m_direction, rollAmount.toRadians());
    m_up = (rollRotation * Vector4f(m_up, 0.0f)).xyz().normalized();
  }

  void Camera::tilt(const Angle& tiltAmount)
  {
    Vector3f right = getRight();
    Matrix4 tiltRotation = Matrix4::RotationAxis(right, tiltAmount.toRadians());
    m_direction = (tiltRotation * Vector4f(m_direction, 0.0f)).xyz().normalized();
    recalculateUp();
  }

  void Camera::pan(const Angle& panAmount)
  {
    Matrix4 panRotation = Matrix4::RotationAxis(m_up, panAmount.toRadians());
    m_direction = (panRotation * Vector4f(m_direction, 0.0f)).xyz().normalized();
  }

  void Camera::dolly(float delta)
  {
    m_position += m_direction * delta;
  }

  void Camera::pedestal(float delta)
  {
    m_position += m_up * delta;
  }

  void Camera::truck(float delta)
  {
    Vector3f right = getRight();
    m_position += right * delta;
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
    recalculateUp();
  }

  void Camera::rotate(const Vector3f& eulerAngles)
  {
    Matrix4 rotationMatrix = Matrix4::Rotation(eulerAngles);
    m_direction = (rotationMatrix * Vector4f(m_direction, 0.0f)).xyz().normalized();
    recalculateUp();
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

  void Camera::recalculateUp()
  {
    Vector3f right = getRight();
    m_up = right.cross(m_direction).normalized();
  }
}
