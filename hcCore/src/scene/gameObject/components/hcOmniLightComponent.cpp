#include "hc/scene/gameObject/components/hcOmniLightComponent.h"
#include "hc/scene/gameObject/hcGameObject.h"

namespace hc
{
  static constexpr UInt16 OMNI_LIGHT_COMPONENT_VERSION = 1;

  OmniLightComponent::OmniLightComponent() :
    ALightComponent(componentType::OmniLight)
  {}

  OmniLightComponent::~OmniLightComponent()
  {}

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

  void OmniLightComponent::onSerialize(io::BinaryWriter& writer) const
  {
    m_omniLight.serialize(writer);
  }

  void OmniLightComponent::onDeserialize(io::BinaryReader& reader)
  {
    m_omniLight.deserialize(reader);
  }

  UInt16 OmniLightComponent::getDerivedVersion() const
  {
    return OMNI_LIGHT_COMPONENT_VERSION;
  }
}
