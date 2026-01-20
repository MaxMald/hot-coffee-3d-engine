#include "hc/hcLight.h"

namespace hc
{
  Light::Light(lightType::Type type) : m_type(type),
      m_color{1.0f, 1.0f, 1.0f},
      m_intensity(1.0f),
      m_position{0.0f, 0.0f, 0.0f},
      m_direction{0.0f, -1.0f, 0.0f}
  {
  }

  void Light::setType(lightType::Type type)
  {
    m_type = type;
  }

  lightType::Type Light::getType() const
  {
    return m_type;
  }

  void Light::setColor(const Color& color)
  {
    m_color = color;
  }

  const Color& Light::getColor() const
  {
    return m_color;
  }

  void Light::setIntensity(float intensity)
  {
    m_intensity = intensity;
  }

  float Light::getIntensity() const
  {
    return m_intensity;
  }

  void Light::setPosition(const Vector3f& position)
  {
    m_position = position;
  }

  const Vector3f& Light::getPosition() const
  {
    return m_position;
  }

  void Light::setDirection(const Vector3f& direction)
  {
    m_direction = direction;
  }

  const Vector3f& Light::getDirection() const
  {
    return m_direction;
  }
}
