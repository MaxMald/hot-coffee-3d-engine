#include "hc/scene/gameObject/components/hcDirectionalLightComponent.h"
#include "hc/scene/gameObject/hcGameObject.h"

namespace hc
{
  DirectionalLightComponent::DirectionalLightComponent() :
    ALightComponent(componentType::DirectionalLight)
  {}

  DirectionalLightComponent::~DirectionalLightComponent()
  {}

  void DirectionalLightComponent::serialize(BinaryWriter& writer) const
  {
    m_directionalLight.serialize(writer);
  }

  void DirectionalLightComponent::deserialize(BinaryReader& reader)
  {
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
    m_directionalLight.setPosition(m_gameObject->getWorldPosition());

    Vector3f worldRotation = m_gameObject->getWorldRotation();
    Matrix4 rotationMatrix = Matrix4::Rotation(worldRotation);
    Vector3f direction = Vector3f(1.0f, 0.0f, 0.0f);
    direction = (rotationMatrix * Vector4f(direction, 0.0f)).xyz();
    m_directionalLight.setDirection(direction);
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
