#pragma once

#include "hc/hcIComponent.h"

namespace hc
{
  class HC_CORE_EXPORT ABaseComponent : public IComponent
  {
  public:
    virtual ~ABaseComponent() override = default;

    void setGameObject(GameObject* gameObject) override;
    GameObject* getGameObject() const override;
    componentType::Type getType() const override;

  protected:
    GameObject* m_gameObject;
    componentType::Type m_type;

    ABaseComponent(componentType::Type type);

    virtual void onGameObjectSet();
  };
}
