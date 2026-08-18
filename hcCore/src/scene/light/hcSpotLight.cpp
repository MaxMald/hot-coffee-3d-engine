#include "hc/scene/light/hcSpotLight.h"

namespace hc
{
  static const float MAX_CONE_ANGLE_RADIANS = Math::HalfPi;

  SpotLight::SpotLight() :
    ALight(lightType::Type::Spot),
    m_direction(0.0f, -1.0f, 0.0f),
    m_innerConeAngle(Angle::FromDegrees(15.0f)),
    m_outerConeAngle(Angle::FromDegrees(30.0f)),
    m_shadowProjectionNearPlane(0.1f),
    m_shadowProjectionFarPlane(100.0f)
  {
  }

  SpotLight::~SpotLight() = default;

  void SpotLight::serialize(BinaryWriter& writer) const
  {
    ALight::serialize(writer);
    writer.writeVector3f(m_direction);
    writer.writeAngle(m_innerConeAngle);
    writer.writeAngle(m_outerConeAngle);

    // TODO
    //
    // serialize shadow settings
  }

  void SpotLight::deserialize(BinaryReader& reader)
  {
    ALight::deserialize(reader);
    m_direction = reader.readVector3f();
    m_innerConeAngle = reader.readAngle();
    m_outerConeAngle = reader.readAngle();

    // TODO
    //
    // deserialize shadow settings
  }

  void SpotLight::setDirection(const Vector3f& direction)
  {
    m_direction = direction;
  }

  const Vector3f& SpotLight::getDirection() const
  {
    return m_direction;
  }

  void SpotLight::setInnerConeAngle(Angle angle)
  {
    float angleRadians = angle.toRadians();
    m_innerConeAngle = Angle::FromRadians(
      Math::Max(0.0f, Math::Min(angleRadians, MAX_CONE_ANGLE_RADIANS))
    );
    m_outerConeAngle = Math::Max(m_outerConeAngle, m_innerConeAngle);
  }

  Angle SpotLight::getInnerConeAngle() const
  {
    return m_innerConeAngle;
  }

  void SpotLight::setOuterConeAngle(Angle angle)
  {
    float angleRadians = angle.toRadians();;
    m_outerConeAngle = Angle::FromRadians(
      Math::Max(0.0f, Math::Min(angleRadians, MAX_CONE_ANGLE_RADIANS))
    );
    m_innerConeAngle = Math::Min(m_innerConeAngle, m_outerConeAngle);
  }

  Angle SpotLight::getOuterConeAngle() const
  {
    return m_outerConeAngle;
  }

  void SpotLight::setShadowProjectionNearPlane(float nearPlane)
  {
    m_shadowProjectionNearPlane = Math::Max(0.0f, nearPlane);
  }

  float SpotLight::getShadowProjectionNearPlane() const
  {
    return m_shadowProjectionNearPlane;
  }

  void SpotLight::setShadowProjectionFarPlane(float farPlane)
  {
    m_shadowProjectionFarPlane = Math::Max(0.0f, farPlane);
  }

  float SpotLight::getShadowProjectionFarPlane() const
  {
    return m_shadowProjectionFarPlane;
  }

  SpotLightFrameData SpotLight::toFrameData() const
  {
    SpotLightFrameData frameData{};
    frameData.position = Vector4f(m_position, 1.0f);
    frameData.direction = Vector4f(m_direction.normalized(), 0.0f);
    frameData.color = m_color;
    frameData.range = m_range;
    frameData.innerConeAngle = Math::Cos(m_innerConeAngle.toRadians());
    frameData.intensity = m_intensity;
    frameData.outerConeAngle = Math::Cos(m_outerConeAngle.toRadians());
    return frameData;
  }
}
