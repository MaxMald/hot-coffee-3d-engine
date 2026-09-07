#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/gameObject/components/hcComponentType.h"

namespace hc
{
  class GameObject;

  /**
   * @brief Interface for components that can be attached to game objects.
   */
  class HC_CORE_EXPORT IComponent : public NonCopyable, public io::ISerializable
  {
  public:
    virtual ~IComponent() = default;

    /**
     * Sets the game object this component is attached to.
     * 
     * @param gameObject Pointer to the parent game object.
     */
    virtual void setGameObject(GameObject* gameObject) = 0;

    /**
     * Gets the game object this component is attached to.
     * 
     * @return Pointer to the parent game object.
     */
    virtual GameObject* getGameObject() const = 0;

    /**
     * Gets the type of this component.
     * 
     * @return The component type identifier.
     */
    virtual componentType::Type getType() const = 0;

  protected:
    IComponent() = default;
  };
}
