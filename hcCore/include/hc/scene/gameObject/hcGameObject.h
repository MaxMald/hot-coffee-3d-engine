#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/hcIDrawable.h"
#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManager.h"

namespace hc
{
  class IGameObjectFactory;
  class IComponent;

  /**
   * @class GameObject
   * @brief Represents an entity in the scene graph with transform, rendering,
   * and component support.
   *
   * GameObject is the base class for all entities in the engine. It supports
   * hierarchical relationships, transformation, rendering, and extensibility via
   * components. Each GameObject can have a parent, multiple children, and
   * multiple components. Ownership of children and components is managed
   * internally.
   *
   * @note GameObject instances are non-copyable.
   */
  class HC_CORE_EXPORT GameObject :
    public NonCopyable,
    public ISerializable,
    public Transform,
    public IDrawable
  {
  public:
    /**
     * @brief Constructs a GameObject with the specified name and factory.
     *
     * @param name The name of the GameObject.
     * @param gameObjectFactory Reference to the factory used for creating child
     * objects.
     * @param componentFactoriesManager Reference to the manager for component
     * factories used to create components.
     */
    GameObject(
      const String& name,
      IGameObjectFactory& gameObjectFactory,
      ComponentFactoriesManager& componentFactoriesManager
    );

    /**
     * @brief Destructor for GameObject.
     */
    virtual ~GameObject();

    /**
     * @copydoc ISerializable::serialize
     */
    void serialize(BinaryWriter& writer) const override;

    /**
     * @copydoc ISerializable::deserialize
     */
    void deserialize(BinaryReader& reader) override;

    /**
     * @brief Renders the GameObject and its drawable children/components.
     *
     * @param renderContext Rendering context for drawing.
     */
    virtual void draw(const RenderContext& renderContext) override;

    /**
     * @brief Updates the GameObject and its children.
     *
     * @param elapsedTime Time elapsed since the last update.
     */
    void update(const Time& elapsedTime);

    /**
     * @brief Sets the name of the GameObject.
     *
     * @param name The new name.
     */
    void setName(const String& name);

    /**
     * @brief Gets the name of the GameObject.
     *
     * @return The name.
     */
    const String& getName() const;

    /**
     * @brief Gets the parent GameObject.
     *
     * @return Pointer to the parent, or nullptr if root.
     */
    GameObject* getParent() const;

    /**
     * @brief Adds a child GameObject and takes ownership.
     *
     * @param child Unique pointer to the child GameObject.
     */
    void addChild(UniquePtr<GameObject> child);

    /**
     * @brief Creates and adds a new child GameObject.
     *
     * @param childName Name for the new child.
     *
     * @return Pointer to the newly created child GameObject.
     */
    GameObject* createChild(const String& childName);

    /**
     * @brief Removes a child GameObject and returns ownership.
     *
     * @param child Pointer to the child to remove.
     *
     * @return Unique pointer to the removed child, or nullptr if not found.
     */
    UniquePtr<GameObject> removeChild(GameObject* child);

    /**
     * @brief Gets the first child GameObject with the specified name.
     *
     * @param name Name to search for.
     *
     * @return Pointer to the matching child, or nullptr if not found.
     */
    GameObject* getChild(const String& name);

    /**
     * @brief Gets all child GameObjects with the specified name.
     *
     * @param name Name to search for.
     *
     * @return Vector of pointers to matching children.
     */
    Vector<GameObject*> getChildrenByName(const String& name);

    /**
     * @brief Gets all children of this GameObject.
     *
     * @return Vector of unique pointers to children.
     */
    const Vector<UniquePtr<GameObject>>& getChildren() const;

    /**
     * @brief Computes the world transformation matrix for this GameObject.
     *
     * @return The world matrix.
     */
    Matrix4 getWorldMatrix() const;

    /**
     * @brief Creates and adds a component of the specified type to this
     * GameObject.
     *
     * @tparam ComponentType The type of component to create.
     *
     * @return Pointer to the created component.
     *
     * @throws RuntimeErrorException if a component of the specified type already
     * exists or if creation fails.
     */
    template<typename ComponentType>
    ComponentType* createComponent();

    /**
     * @brief Checks if a component of the specified type is attached to this
     * GameObject.
     *
     * @return True if a component of the specified type exists, false otherwise.
     */
    template<typename ComponentType>
    bool hasComponent() const;

    /**
     * @brief Gets all components attached to this GameObject.
     * 
     * @return Vector of pointers to all components. The order of components in
     *         the returned vector is unspecified and should not be relied upon.
     */
    Vector<IComponent*> getComponents() const;

    /**
     * @brief Gets all components attached to this GameObject and appends them to
     * the provided vector. The vector will be cleared before adding components.
     *
     * @param outComponents Vector to which component pointers will be appended.
     * The order of components in the provided vector after the call is
     * unspecified and should not be relied upon.
     */
    void getComponents(Vector<IComponent*>& outComponents) const;

  private:
    String m_name;
    GameObject* m_parent = nullptr;
    IGameObjectFactory& m_gameObjectFactory;
    ComponentFactoriesManager& m_componentFactoriesManager;
    Vector<UniquePtr<GameObject>> m_children;
    UnorderedMap<TypeIndex, UniquePtr<IComponent>> m_components;
    Vector<IDrawable*> m_drawableComponents;

    template<typename ComponentType>
    void addComponent(UniquePtr<ComponentType> component);

    /**
     * @brief Destroys this GameObject and all its children, releasing resources.
     */
    void destroy();
  };

  template<typename ComponentType>
  ComponentType* GameObject::createComponent()
  {
    if (hasComponent<ComponentType>())
    {
      String typeName = typeid(ComponentType).name();
      throw RuntimeErrorException(
        "Component of type " + typeName +
        " already exists on this GameObject."
      );
    }

    UniquePtr<ComponentType> component = m_componentFactoriesManager
      .createComponent<ComponentType>();

    ComponentType* componentPtr = component.get();
    addComponent(std::move(component));
    return componentPtr;
  }

  template<typename ComponentType>
  bool GameObject::hasComponent() const
  {
    TypeIndex typeIndex(typeid(ComponentType));
    return m_components.find(typeIndex) != m_components.end();
  }

  template<typename ComponentType>
  void GameObject::addComponent(UniquePtr<ComponentType> component)
  {
    TypeIndex typeIndex(typeid(ComponentType));

    ComponentType* componentPtr = component.get();
    m_components[typeIndex] = std::move(component);
    IDrawable* drawable = dynamic_cast<IDrawable*>(componentPtr);
    if (drawable)
      m_drawableComponents.push_back(drawable);
  }
}
