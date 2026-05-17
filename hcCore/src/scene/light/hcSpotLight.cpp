#include "hc/scene/light/hcSpotLight.h"

namespace hc
{
  SpotLight::SpotLight() :
    ALight(lightType::Type::Spot),
    m_direction(0.0f, -1.0f, 0.0f),
    m_innerConeAngle(15.0f),
    m_outerConeAngle(30.0f)
  {
  }

  SpotLight::~SpotLight() = default;

  void SpotLight::serialize(BinaryWriter& writer) const
  {
    ALight::serialize(writer);
    writer.writeVector3f(m_direction);
    writer.writeFloat(m_innerConeAngle);
    writer.writeFloat(m_outerConeAngle);
  }

  void SpotLight::deserialize(BinaryReader& reader)
  {
    ALight::deserialize(reader);
    m_direction = reader.readVector3f();
    m_innerConeAngle = reader.readFloat();
    m_outerConeAngle = reader.readFloat();
  }

  void SpotLight::setDirection(const Vector3f& direction)
  {
    m_direction = direction;
  }

  const Vector3f& SpotLight::getDirection() const
  {
    return m_direction;
  }

  void SpotLight::setInnerConeAngle(float angle)
  {
    angle = Math::Max(0.0f, Math::Min(angle, m_outerConeAngle));
    m_innerConeAngle = angle;
  }

  float SpotLight::getInnerConeAngle() const
  {
    return m_innerConeAngle;
  }

  void SpotLight::setOuterConeAngle(float angle)
  {
    angle = Math::Max(0.0f, Math::Min(angle, 180.0f));
    m_outerConeAngle = angle;
    m_innerConeAngle = Math::Min(m_innerConeAngle, m_outerConeAngle);
  }

  float SpotLight::getOuterConeAngle() const
  {
    return m_outerConeAngle;
  }
}
