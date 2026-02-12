#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIDrawable.h"
#include "hc/hcComponentFactoriesManager.h"

namespace hc
{
  class IGameObjectFactory;

  /**
   * @brief Represents an entity in the scene graph with transform, hierarchy,
   * and component support.
   *
   * GameObject is the fundamental building block for all objects in the engine's
   * scene. It supports hierarchical parent-child relationships, transformation,
   * and rendering.
   */
  class HC_CORE_EXPORT GameObject :
    public NonCopyable,
    public Transform,
    public IDrawable
  {
  public:
    /**
     * @brief Constructs a GameObject with a name.
     *
     * @param gameObjectFactory Reference to the GameObject factory for creating
     * child GameObjects.
     * @param componentFactoriesManager Reference to the component factories
     * manager.
     * @param name The name of the GameObject.
     */
    GameObject(
      IGameObjectFactory& gameObjectFactory,
      ComponentFactoriesManager& componentFactoriesManager,
      const String& name
    );

    virtual ~GameObject();

    /**
     * @brief Draws the game object and its children.
     *
     * @param renderContext The rendering context for drawing.
     */
    virtual void draw(const RenderContext& renderContext) override;

    /**
     * @brief Updates the game object and its children.
     *
     * @param elapsedTime Time elapsed since last frame (in seconds).
     */
    void update(const Time& elapsedTime);

    /**
     * @brief Sets the GameObject name.
     *
     * @param name The new name for the GameObject.
     */
    void setName(const String& name);

    /**
     * @brief Gets the GameObject name.
     *
     * @return The name of the GameObject.
     */
    const String& getName() const;

    /**
     * @brief Gets the parent GameObject.
     *
     * @return Pointer to the parent GameObject, or nullptr if this is a root
     * object.
     */
    GameObject* getParent() const;

    /**
     * @brief Adds a child GameObject.
     *
     * @param child The child GameObject to add.
     */
    void addChild(UniquePtr<GameObject> child);

    /**
     * @brief Creates a new child GameObject with the specified name and adds it
     * to this GameObject.
     *
     * @param name The name of the child GameObject to create.
     *
     * @return Pointer to the created child GameObject.
     */
    GameObject* createChild(const String& name);

    /**
     * @brief Gets the first child GameObject with the specified name.
     *
     * @param name The name of the child GameObject to retrieve.
     *
     * @return Pointer to the first child GameObject with the specified name, or
     * nullptr if not found.
     */
    GameObject* getChild(const String& name);

    /**
     * @brief Removes a child GameObject. Returns ownership to the caller if
     * found, nullptr otherwise.
     *
     * @param child Pointer to the child GameObject to remove.
     *
     * @return Unique pointer to the removed child, or nullptr if not found.
     */
    UniquePtr<GameObject> removeChild(GameObject* child);

    /**
     * @brief Gets the children of this GameObject.
     *
     * @return A const reference to the vector of child GameObjects.
     */
    const Vector<UniquePtr<GameObject>>& getChildren() const;

    /**
     * @brief Gets the world transformation matrix. Combines this object's
     * transform with all ancestors.
     *
     * @return The world transformation matrix.
     */
    Matrix4 getWorldMatrix() const;

    /**
     * @brief Adds a component to this GameObject. The GameObject takes ownership
     * of the component.
     *
     * Only one component of each type is allowed per GameObject. Adding a
     * duplicate type will throw.
     *
     * @tparam ComponentType The type of component to add.
     *
     * @param component Unique pointer to the component instance.
     *
     * @throws InvalidArgumentException if the component is null.
     * @throws RuntimeErrorException if a component of the same type already
     * exists.
     */
    template<typename ComponentType>
    void addComponent(UniquePtr<ComponentType> component);

    /**
     * @brief Creates and adds a component of the specified type to this
     * GameObject.
     *
     * Only one component of each type is allowed per GameObject.
     *
     * @tparam ComponentType The type of component to create and add.
     *
     * @return A reference to the created component of the specified type.
     *
     * @throws RuntimeErrorException if creation fails or a component of the same
     * type already exists.
     */
    template<typename ComponentType>
    ComponentType& createComponent();

    /**
     * @brief Gets the component of the specified type attached to this
     * GameObject.
     *
     * @tparam ComponentType The type of component to retrieve.
     *
     * @return A reference to the component of the specified type.
     *
     * @throws RuntimeErrorException if no component of the specified type is
     * found.
     */
    template<typename ComponentType>
    ComponentType& getComponent();

    /**
     * @brief Checks if this GameObject has a component of the specified type.
     *
     * @tparam ComponentType The type of component to check for.
     *
     * @return true if a component of the specified type is attached, false
     * otherwise.
     */
    template<typename ComponentType>
    bool hasComponent() const;

    /**
     * @brief Gets all components attached to this GameObject.
     *
     * @return A vector of raw pointers to the components.
     */
    Vector<IComponent*> getComponents() const;

  private:
    String m_name;
    GameObject* m_parent = nullptr;
    Vector<UniquePtr<GameObject>> m_children;
    UnorderedMap<TypeIndex, UniquePtr<IComponent>> m_components;
    Vector<IDrawable*> m_drawableComponents;
    IGameObjectFactory& m_gameObjectFactory;
    ComponentFactoriesManager& m_componentFactoriesManager;

    /**
     * @brief Registers the component as drawable if it implements IDrawable.
     *
     * @param component Pointer to the component to check and register.
     */
    void tryRegisterIDrawableComponent(IComponent* component);
  };

  template<typename ComponentType>
  void GameObject::addComponent(UniquePtr<ComponentType> component)
  {
    static_assert(
      std::derived_from<ComponentType, IComponent>,
      "ComponentType must be derived from IComponent"
      );

    if (!component)
    {
      throw InvalidArgumentException(
        "Component cannot be null when adding to GameObject."
      );
    }

    if (hasComponent<ComponentType>())
    {
      throw RuntimeErrorException(
        "GameObject already has a component of type: " + std::string(typeid(ComponentType).name())
      );
    }

    ComponentType* componentPtr = component.get();
    componentPtr->setGameObject(this);

    TypeIndex typeIndex = TypeIndex::get<ComponentType>();
    m_components[typeIndex] = std::move(component);

    tryRegisterIDrawableComponent(componentPtr);
  }

  template<typename ComponentType>
  ComponentType& GameObject::createComponent()
  {
    static_assert(
      std::derived_from<ComponentType, IComponent>,
      "ComponentType must be derived from IComponent"
      );

    UniquePtr<ComponentType> component = m_componentFactoriesManager
      .createComponent<ComponentType>();

    if (!component)
    {
      throw RuntimeErrorException(
        "Failed to create component of type: " + String(typeid(ComponentType).name())
      );
    }

    addComponent(std::move(component));
    return getComponent<ComponentType>();
  }

  template<typename ComponentType>
  ComponentType& GameObject::getComponent()
  {
    static_assert(
      std::derived_from<ComponentType, IComponent>,
      "ComponentType must be derived from IComponent"
      );

    TypeIndex typeIndex = TypeIndex::get<ComponentType>();
    auto it = m_components.find(typeIndex);
    if (it == m_components.end())
    {
      throw RuntimeErrorException(
        "Component of type: " + String(typeid(ComponentType).name()) + " not found in GameObject."
      );
    }

    IComponent* componentPtr = it->second.get();
    if (!componentPtr)
    {
      throw RuntimeErrorException(
        "Component of type: " + String(typeid(ComponentType).name()) + " is null in GameObject."
      );
    }

    ComponentType* desiredComponentType = dynamic_cast<ComponentType*>(componentPtr);
    if (!desiredComponentType)
    {
      throw RuntimeErrorException(
        "Component of type: " + String(typeid(ComponentType).name()) + " found but is not of the expected type in GameObject."
      );
    }

    return *desiredComponentType;
  }

  template<typename ComponentType>
  bool GameObject::hasComponent() const
  {
    static_assert(
      std::derived_from<ComponentType, IComponent>,
      "ComponentType must be derived from IComponent"
      );

    TypeIndex typeIndex = TypeIndex::get<ComponentType>();
    return m_components.contains(typeIndex);
  }
}
