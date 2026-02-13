#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIDrawable.h"

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
     */
    GameObject(const String& name, IGameObjectFactory& gameObjectFactory);

    /**
     * @brief Destructor for GameObject.
     */
    virtual ~GameObject();

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
     * @brief Adds a component to this GameObject and takes ownership.
     * 
     * @param component Unique pointer to the component.
     */
    void addComponent(UniquePtr<IComponent> component);

    /**
     * @brief Gets all components attached to this GameObject.
     * 
     * @return Vector of unique pointers to components.
     */
    const Vector<UniquePtr<IComponent>>& getComponents() const;

  private:
    String m_name;
    GameObject* m_parent = nullptr;
    IGameObjectFactory& m_gameObjectFactory;
    Vector<UniquePtr<GameObject>> m_children;
    Vector<UniquePtr<IComponent>> m_components;
    Vector<IDrawable*> m_drawableComponents;
  };
}
