#include "hc/scene/light/hcDirectionalLight.h"

namespace hc
{
  DirectionalLight::DirectionalLight() :
    ALight(lightType::Type::Directional),
    m_direction(0.0f, -1.0f, 0.0f),
    m_shadowViewTarget(0.0f, 0.0f, 0.0f),
    m_shadowViewDistance(50.0f),
    m_shadowProjectionSize(10.0f),
    m_shadowProjectionNearPlane(0.1f),
    m_shadowProjectionFarPlane(100.0f)
  {}

  DirectionalLight::~DirectionalLight() = default;

  void DirectionalLight::serialize(BinaryWriter& writer) const
  {
    ALight::serialize(writer);
    writer.writeVector3f(m_direction);

    // TODO
    //
    // We should serialize the shadow projection parameters as well.
  }

  void DirectionalLight::deserialize(BinaryReader& reader)
  {
    ALight::deserialize(reader);
    m_direction = reader.readVector3f();

    // TODO
    //
    // We should deserialize the shadow projection parameters as well.
  }

  void DirectionalLight::setDirection(const Vector3f& direction)
  {
    m_direction = direction;
  }

  const Vector3f& DirectionalLight::getDirection() const
  {
    return m_direction;
  }

  void DirectionalLight::setShadowViewTarget(const Vector3f& target)
  {
    m_shadowViewTarget = target;
  }

  const Vector3f& DirectionalLight::getShadowViewTarget() const
  {
    return m_shadowViewTarget;
  }

  void DirectionalLight::setShadowViewDistance(float distance)
  {
    m_shadowViewDistance = Math::Max(distance, 0.1f);
  }

  float DirectionalLight::getShadowViewDistance() const
  {
      return m_shadowViewDistance;
  }

  void DirectionalLight::setShadowProjectionSize(float size)
  {
    m_shadowProjectionSize = Math::Max(size, 0.1f);
  }

  float DirectionalLight::getShadowProjectionSize() const
  {
    return m_shadowProjectionSize;
  }

  void DirectionalLight::setShadowProjectionNearPlane(float nearPlane)
  {
    m_shadowProjectionNearPlane = Math::Max(nearPlane, 0.1f);
  }

  float DirectionalLight::getShadowProjectionNearPlane() const
  {
    return m_shadowProjectionNearPlane;
  }

  void DirectionalLight::setShadowProjectionFarPlane(float farPlane)
  {
    m_shadowProjectionFarPlane = Math::Max(farPlane, 0.1f);
  }

  float DirectionalLight::getShadowProjectionFarPlane() const
  {
    return m_shadowProjectionFarPlane;
  }

  DirectionalLightFrameData DirectionalLight::toFrameData() const
  {
    DirectionalLightFrameData frameData;
    frameData.directionAndIntensity = Vector4f(m_direction.normalized(), m_intensity);
    frameData.color = m_color;
    return frameData;
  }
}
