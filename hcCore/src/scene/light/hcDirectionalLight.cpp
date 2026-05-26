#include "hc/scene/light/hcDirectionalLight.h"

namespace hc
{
  DirectionalLight::DirectionalLight() :
    ALight(lightType::Type::Directional),
    m_direction(0.0f, -1.0f, 0.0f)
  {
  }

  DirectionalLight::~DirectionalLight() = default;

  void DirectionalLight::serialize(BinaryWriter& writer) const
  {
    ALight::serialize(writer);
    writer.writeVector3f(m_direction);
  }

  void DirectionalLight::deserialize(BinaryReader& reader)
  {
    ALight::deserialize(reader);
    m_direction = reader.readVector3f();
  }

  void DirectionalLight::setDirection(const Vector3f& direction)
  {
    m_direction = direction;
  }

  const Vector3f& DirectionalLight::getDirection() const
  {
    return m_direction;
  }

  DirectionalLightFrameData DirectionalLight::toFrameData() const
  {
    DirectionalLightFrameData frameData;
    frameData.directionAndIntensity = Vector4f(m_direction.normalized(), m_intensity);
    frameData.color = m_color;
    return frameData;
  }
}
