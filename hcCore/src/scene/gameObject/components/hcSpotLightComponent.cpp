#include "hc/scene/gameObject/components/hcSpotLightComponent.h"
#include "hc/scene/gameObject/hcGameObject.h"

namespace hc
{
  SpotLightComponent::SpotLightComponent() :
    ALightComponent(componentType::SpotLight)
  {}

  SpotLightComponent::~SpotLightComponent()
  {}

  void SpotLightComponent::serialize(BinaryWriter& writer) const
  {
    m_spotLight.serialize(writer);
  }

  void SpotLightComponent::deserialize(BinaryReader& reader)
  {
    m_spotLight.deserialize(reader);
  }

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

    Vector3f worldRotation = m_gameObject->getWorldRotation();
    Matrix4 rotationMatrix = Matrix4::Rotation(worldRotation);
    Vector3f direction = Vector3f(1.0f, 0.0f, 0.0f);
    direction = (rotationMatrix * Vector4f(direction, 0.0f)).xyz();
    m_spotLight.setDirection(direction);
  }

  SpotLight& SpotLightComponent::getSpotLight()
  {
    return m_spotLight;
  }

  const SpotLight& SpotLightComponent::getSpotLight() const
  {
    return m_spotLight;
  }
}
