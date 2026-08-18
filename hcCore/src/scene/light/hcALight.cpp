#include "hc/scene/light/hcALight.h"

namespace hc
{
  void ALight::serialize(BinaryWriter& writer) const
  {
    writer.writeBool(m_enabled);
    writer.writeUInt8(static_cast<UInt8>(m_type));
    writer.writeColor(m_color);
    writer.writeFloat(m_intensity);
    writer.writeFloat(m_range);
    writer.writeVector3f(m_position);
    // TODO serialize shadowEnabled, shadowBias, shadowStrength
  }

  void ALight::deserialize(BinaryReader& reader)
  {
    m_enabled = reader.readBool();
    m_type = static_cast<lightType::Type>(reader.readUInt8());
    m_color = reader.readColor();
    m_intensity = reader.readFloat();
    m_range = reader.readFloat();
    m_position = reader.readVector3f();
    // TODO deserialize shadowEnabled, shadowBias, shadowStrength
  }

  lightType::Type ALight::getType() const
  {
    return m_type;
  }

  void ALight::setColor(const Color& color)
  {
    m_color = color;
  }

  void ALight::setColor(float r, float g, float b)
  {
    m_color = Color(r, g, b);
  }

  const Color& ALight::getColor() const
  {
    return m_color;
  }

  void ALight::setIntensity(float intensity)
  {
    m_intensity = Math::Max(0.0f, intensity);
  }

  float ALight::getIntensity() const
  {
    return m_intensity;
  }

  void ALight::setRange(float range)
  {
    m_range = Math::Max(0.0f, range);
  }

  float ALight::getRange() const
  {
    return m_range;
  }

  void ALight::setPosition(const Vector3f& position)
  {
    m_position = position;
  }

  const Vector3f& ALight::getPosition() const
  {
    return m_position;
  }

  void ALight::setEnabled(bool isEnabled)
  {
    m_enabled = isEnabled;
  }

  bool ALight::isEnabled() const
  {
    return m_enabled;
  }

  void ALight::setShadowsEnabled(bool isEnabled)
  {
    m_shadowsEnabled = isEnabled;
  }

  bool ALight::isShadowsEnabled() const
  {
    return m_shadowsEnabled;
  }

  void ALight::setShadowBias(float bias)
  {
    m_shadowBias = Math::Max(0.0f, bias);
  }

  float ALight::getShadowBias() const
  {
    return m_shadowBias;
  }

  void ALight::setShadowStrength(float strength)
  {
    m_shadowStrength = Math::Clamp(strength, 0.0f, 1.0f);
  }

  float ALight::getShadowStrength() const
  {
    return m_shadowStrength;
  }

  ALight::ALight(lightType::Type type) :
    m_color{ 1.0f, 1.0f, 1.0f },
    m_position{ 0.0f, 0.0f, 0.0f },
    m_intensity(1.0f),
    m_range(1.0f),
    m_shadowBias(0.005f),
    m_shadowStrength(1.0f),
    m_type(type),
    m_enabled(true),
    m_shadowsEnabled(false)
  {}
}
