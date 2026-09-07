#include "hc/scene/light/hcALight.h"

namespace hc
{
  static constexpr UInt16 ALIGHT_VERSION = 1;

  void ALight::serialize(io::BinaryWriter& writer) const
  {
    UInt32 composedVersion = (static_cast<UInt32>(ALIGHT_VERSION) << 16) | static_cast<UInt32>(getDerivedVersion());
    writer.startWritingObject(composedVersion);
    writer.writeBool(m_enabled);
    writer.writeUInt8(static_cast<UInt8>(m_type));
    writer.writeColor(m_color);
    writer.writeFloat(m_intensity);
    writer.writeFloat(m_range);
    writer.writeVector3f(m_position);
    writer.writeBool(m_shadowsEnabled);
    writer.writeFloat(m_shadowBias);
    writer.writeFloat(m_shadowStrength);

    onSerialize(writer);
    writer.finishWritingObject();
  }

  void ALight::deserialize(io::BinaryReader& reader)
  {
    io::ObjectHeader header = reader.startReadingObject();
    UInt32 composedVersion = (static_cast<UInt32>(ALIGHT_VERSION) << 16) | static_cast<UInt32>(getDerivedVersion());
    if (!header.match(composedVersion))
    {
      reader.finishReadingObject();
      return;
    }

    m_enabled = reader.readBool();
    m_type = static_cast<lightType::Type>(reader.readUInt8());
    m_color = reader.readColor();
    m_intensity = reader.readFloat();
    m_range = reader.readFloat();
    m_position = reader.readVector3f();
    m_shadowsEnabled = reader.readBool();
    m_shadowBias = reader.readFloat();
    m_shadowStrength = reader.readFloat();

    onDeserialize(reader);
    reader.finishReadingObject();
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
