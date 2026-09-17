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
     * @copydoc IComponent::getUUID
     */
    inline const UUID& getUUID() const override
    { return m_uuid; }

    /**
     * @copydoc ISerializable::serialize
     */
    void serialize(io::BinaryWriter& writer) const override;

    /**
     * @copydoc ISerializable::deserialize
     */
    void deserialize(io::BinaryReader& reader) override;

  protected:
    UUID m_uuid;
    GameObject* m_gameObject;
    componentType::Type m_type;

    /**
     * Constructs a base component with the specified type.
     * 
     * @param type The component type identifier.
     */
    ABaseComponent(componentType::Type type);

    /**
     * Hook for derived classes to implement their specific serialization logic.
     *
     * @param writer The BinaryWriter to use for serialization.
     */
    virtual void onSerialize(io::BinaryWriter& writer) const = 0;

    /**
     * Hook for derived classes to implement their specific deserialization logic.
     *
     * @param reader The BinaryReader to use for deserialization.
     */
    virtual void onDeserialize(io::BinaryReader& reader) = 0;

    /**
     * Returns the version number for the derived component class.
     *
     * Derived classes should override this to provide a unique version
     * number for serialization compatibility checks.
     *
     * @return The version number of the derived component class.
     */
    virtual UInt16 getDerivedVersion() const = 0;

    /**
     * Called when the game object is set.
     * 
     * Derived classes can override this to perform initialization that
     * requires access to the game object.
     */
    virtual void onGameObjectSet();
  };
}
