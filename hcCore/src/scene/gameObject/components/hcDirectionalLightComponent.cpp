#include "hc/scene/gameObject/components/hcDirectionalLightComponent.h"
#include "hc/scene/gameObject/hcGameObject.h"

namespace hc
{
  static constexpr UInt16 DIRECTIONAL_LIGHT_COMPONENT_VERSION = 1;

  DirectionalLightComponent::DirectionalLightComponent() :
    ALightComponent(componentType::DirectionalLight)
  {}

  DirectionalLightComponent::~DirectionalLightComponent()
  {}

  void DirectionalLightComponent::preUpdate(float)
  {
    updateLight();
  }

  void DirectionalLightComponent::update(float)
  {}

  void DirectionalLightComponent::postUpdate(float)
  {
    updateLight();
  }

  ALight& DirectionalLightComponent::getLight()
  {
    return m_directionalLight;
  }

  const ALight& DirectionalLightComponent::getLight() const
  {
    return m_directionalLight;
  }

  void DirectionalLightComponent::updateLight()
  {
    Matrix4 worldRotation = m_gameObject->getWorldRotationMatrix();
    Vector3f direction = Vector3f(0.0f, 0.0f, -1.0f);
    m_directionalLight.setDirection((worldRotation * Vector4f(direction, 0.0f)).xyz());
    m_directionalLight.setPosition(m_gameObject->getWorldPosition());
  }

  DirectionalLight& DirectionalLightComponent::getDirectionalLight()
  {
    return m_directionalLight;
  }

  const DirectionalLight& DirectionalLightComponent::getDirectionalLight() const
  {
    return m_directionalLight;
  }

  void DirectionalLightComponent::onSerialize(io::BinaryWriter& writer) const
  {
    m_directionalLight.serialize(writer);
  }

  void DirectionalLightComponent::onDeserialize(io::BinaryReader& reader)
  {
    m_directionalLight.deserialize(reader);
  }

  UInt16 DirectionalLightComponent::getDerivedVersion() const
  {
    return DIRECTIONAL_LIGHT_COMPONENT_VERSION;
  }
}
