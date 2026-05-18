#pragma once

#include "hc/scene/gameObject/components/hcIComponent.h"

namespace hc
{
  /**
   * Abstract base class providing common functionality for components.
   * 
   * ABaseComponent implements the IComponent interface and provides
   * default implementations for component management, including game
   * object association, type identification, and serialization support.
   * Derived classes must implement the serialization hooks and specify
   * their component type.
   */
  class HC_CORE_EXPORT ABaseComponent : public IComponent
  {
  public:
    /*
     * Destructs the base component. This is a virtual destructor to ensure
     * proper cleanup of derived classes when deleted through a base pointer.
     */
    virtual ~ABaseComponent() override = default;

    /**
     * @copydoc IComponent::setGameObject
     */
    void setGameObject(GameObject* gameObject) override;

    /**
     * @copydoc IComponent::getGameObject
     */
    GameObject* getGameObject() const override;

    /**
     * @copydoc IComponent::getType
     */
    componentType::Type getType() const override;

    /**
     * @copydoc ISerializable::serialize
     */
    virtual void serialize(BinaryWriter& writer) const override;

    /**
     * @copydoc ISerializable::deserialize
     */
    virtual void deserialize(BinaryReader& reader) override;

  protected:
    GameObject* m_gameObject;
    componentType::Type m_type;

    /**
     * Constructs a base component with the specified type.
     * 
     * @param type The component type identifier.
     */
    ABaseComponent(componentType::Type type);

    /**
     * Called when the game object is set.
     * 
     * Derived classes can override this to perform initialization that
     * requires access to the game object.
     */
    virtual void onGameObjectSet();
  };
}
