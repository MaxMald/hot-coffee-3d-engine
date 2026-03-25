#include "hc/scene/light/hcLight.h"

namespace hc
{
  Light::Light(lightType::Type type) :
    m_id(UUID::Generate()),
    enabled(true),
    m_type(type),
    m_color{ 1.0f, 1.0f, 1.0f },
    m_intensity(1.0f),
    m_position{ 0.0f, 0.0f, 0.0f },
    m_direction{ 0.0f, -1.0f, 0.0f }
  {
  }

  void Light::serialize(BinaryWriter& writer) const
  {
    m_id.serialize(writer);
    writer.writeBool(enabled);
    writer.writeUInt8(static_cast<UInt8>(m_type));
    writer.writeColor(m_color);
    writer.writeFloat(m_intensity);
    writer.writeVector3f(m_position);
    writer.writeVector3f(m_direction);
  }

  void Light::deserialize(BinaryReader& reader)
  {
    m_id.deserialize(reader);
    enabled = reader.readBool();
    m_type = static_cast<lightType::Type>(reader.readUInt8());
    m_color = reader.readColor();
    m_intensity = reader.readFloat();
    m_position = reader.readVector3f();
    m_direction = reader.readVector3f();
  }

  const UUID& Light::getUUID() const
  {
    return m_id;
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

  void Light::setColor(float r, float g, float b)
  {
    m_color = Color(r, g, b);
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

  void Light::setEnabled(bool isEnabled)
  {
    enabled = isEnabled;
  }

  bool Light::isEnabled() const
  {
    return enabled;
  }
}
