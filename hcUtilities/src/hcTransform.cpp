#include "hc/hcTransform.h"

namespace hc
{
  Transform::Transform() :
    m_position(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f),
    m_rotation(0.0f, 0.0f, 0.0f)
  {
  }

  Transform::~Transform() = default;

  void Transform::setScale(const Vector3f& scaleFactors)
  {
    m_scale = scaleFactors;
  }

  void Transform::setScale(float x, float y, float z)
  {
    m_scale.x = x;
    m_scale.y = y;
    m_scale.z = z;
  }

  const Vector3f& Transform::getScale() const
  {
    return m_scale;
  }

  void Transform::setPosition(const Vector3f& position)
  {
    m_position = position;
  }

  void Transform::setPosition(float x, float y, float z)
  {
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
  }

  const Vector3f& Transform::getPosition() const
  {
    return m_position;
  }

  void Transform::setRotation(const Vector3f& eulerAngles)
  {
    m_rotation = eulerAngles;
  }

  void Transform::setRotation(float x, float y, float z)
  {
    m_rotation.x = x;
    m_rotation.y = y;
    m_rotation.z = z;
  }

  const Vector3f& Transform::getRotation() const
  {
    return m_rotation;
  }

  Matrix4 Transform::getMatrix() const
  {
    Matrix4 translation = Matrix4::Translate(m_position);
    Matrix4 scale = Matrix4::Scale(m_scale);
    Matrix4 rotation = Matrix4::Rotation(m_rotation);

    return translation * rotation * scale;
  }
}
