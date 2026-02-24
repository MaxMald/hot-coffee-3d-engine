#include "hc/scene/gameObject/components/hcABaseComponent.h"

namespace hc
{
  ABaseComponent::ABaseComponent(componentType::Type type) :
    m_gameObject(nullptr),
    m_type(type)
  {
  }

  void ABaseComponent::setGameObject(GameObject* gameObject)
  {
    m_gameObject = gameObject;
    onGameObjectSet();
  }

  GameObject* ABaseComponent::getGameObject() const
  {
    return m_gameObject;
  }

  componentType::Type ABaseComponent::getType() const
  {
    return m_type;
  }

  void ABaseComponent::onGameObjectSet()
  {
    // Default implementation does nothing.
  }
}
