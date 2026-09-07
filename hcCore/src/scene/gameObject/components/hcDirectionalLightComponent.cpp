#include "hc/scene/gameObject/components/hcDirectionalLightComponent.h"
#include "hc/scene/gameObject/hcGameObject.h"

namespace hc
{
  DirectionalLightComponent::DirectionalLightComponent() :
    ALightComponent(componentType::DirectionalLight)
  {}

  DirectionalLightComponent::~DirectionalLightComponent()
  {}

  void DirectionalLightComponent::serialize(io::BinaryWriter& writer) const
  {
    ABaseComponent::serialize(writer);
    m_directionalLight.serialize(writer);
  }

  void DirectionalLightComponent::deserialize(io::BinaryReader& reader)
  {
    ABaseComponent::deserialize(reader);
    m_directionalLight.deserialize(reader);
  }

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
}
