#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/gameObject/components/hcComponentType.h"

namespace hc
{
  class GameObject;

  class HC_CORE_EXPORT IComponent : public NonCopyable
  {
  public:
    virtual ~IComponent() = default;

    virtual void setGameObject(GameObject* gameObject) = 0;
    virtual GameObject* getGameObject() const = 0;
    virtual componentType::Type getType() const = 0;

  protected:
    IComponent() = default;
  };
}
