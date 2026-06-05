#include "hc/scene/gameObject/components/hcOmniLightComponent.h"
#include "hc/scene/gameObject/hcGameObject.h"

namespace hc
{
  OmniLightComponent::OmniLightComponent() :
    ALightComponent(componentType::OmniLight)
  {}

  OmniLightComponent::~OmniLightComponent()
  {}

  void OmniLightComponent::serialize(BinaryWriter& writer) const
  {
    ABaseComponent::serialize(writer);
    m_omniLight.serialize(writer);
  }

  void OmniLightComponent::deserialize(BinaryReader& reader)
  {
    ABaseComponent::deserialize(reader);
    m_omniLight.deserialize(reader);
  }

  void OmniLightComponent::preUpdate(float)
  {
    updateLight();
  }

  void OmniLightComponent::update(float)
  {}

  void OmniLightComponent::postUpdate(float)
  {
    updateLight();
  }

  ALight& OmniLightComponent::getLight()
  {
    return m_omniLight;
  }

  const ALight& OmniLightComponent::getLight() const
  {
    return m_omniLight;
  }

  void OmniLightComponent::updateLight()
  {
    m_omniLight.setPosition(m_gameObject->getWorldPosition());
  }

  OmniLight& OmniLightComponent::getOmniLight()
  {
    return m_omniLight;
  }

  const OmniLight& OmniLightComponent::getOmniLight() const
  {
    return m_omniLight;
  }
}
