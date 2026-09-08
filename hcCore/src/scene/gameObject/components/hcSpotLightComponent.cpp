#include "hc/scene/gameObject/components/hcSpotLightComponent.h"
#include "hc/scene/gameObject/hcGameObject.h"

namespace hc
{
  static constexpr UInt16 SPOT_LIGHT_COMPONENT_VERSION = 1;

  SpotLightComponent::SpotLightComponent() :
    ALightComponent(componentType::SpotLight)
  {}

  SpotLightComponent::~SpotLightComponent()
  {}

  void SpotLightComponent::preUpdate(float)
  {
    updateLight();
  }

  void SpotLightComponent::update(float)
  {}

  void SpotLightComponent::postUpdate(float)
  {
    updateLight();
  }

  ALight& SpotLightComponent::getLight()
  {
    return m_spotLight;
  }

  const ALight& SpotLightComponent::getLight() const
  {
    return m_spotLight;
  }

  void SpotLightComponent::updateLight()
  {
    m_spotLight.setPosition(m_gameObject->getWorldPosition());

    Matrix4 worldRotation = m_gameObject->getWorldRotationMatrix();
    Vector3f direction = Vector3f(0.0f, 0.0f, -1.0f);
    m_spotLight.setDirection((worldRotation * Vector4f(direction, 0.0f)).xyz());
  }

  SpotLight& SpotLightComponent::getSpotLight()
  {
    return m_spotLight;
  }

  const SpotLight& SpotLightComponent::getSpotLight() const
  {
    return m_spotLight;
  }

  void SpotLightComponent::onSerialize(io::BinaryWriter& writer) const
  {
    m_spotLight.serialize(writer);
  }

  void SpotLightComponent::onDeserialize(io::BinaryReader& reader)
  {
    m_spotLight.deserialize(reader);
  }

  UInt16 SpotLightComponent::getDerivedVersion() const
  {
    return SPOT_LIGHT_COMPONENT_VERSION;
  }
}
