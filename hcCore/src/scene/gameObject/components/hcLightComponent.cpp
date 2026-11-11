#include "hc/scene/gameObject/components/hcLightComponent.h"
#include "hc/scene/gameObject/hcGameObject.h"

namespace hc
{
  LightComponent::LightComponent() :
    ABaseComponent(componentType::Light),
    m_light()
  {
  }

  LightComponent::~LightComponent() = default;

  void LightComponent::updateLight()
  {
    m_light.position = getGameObject()->getWorldPosition();
  }

  void LightComponent::setLightType(scene::lights::lightType::Type type)
  {
    if (m_light.getType() == type)
      return;
    m_light.setType(type);
  }
}
