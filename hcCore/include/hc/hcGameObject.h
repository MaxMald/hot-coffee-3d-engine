#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIDrawable.h"

namespace hc
{
  class IComponent;

  /**
   * @brief Base class for all game objects in the engine.
   */
  class HC_CORE_EXPORT GameObject :
    public NonCopyable,
    public Transform,
    public IDrawable
  {
  public:
    GameObject();
    GameObject(const String& name);
    virtual ~GameObject();

    /**
     * @brief Draws the game object and its children.
     */
    virtual void draw(const RenderContext& renderContext) override;

    /**
     * @brief Updates the game object and its children.
     * 
     * @param elapsedTime Time elapsed since last frame.
     */
    void update(const Time& elapsedTime);

    /**
     * @brief Sets the GameObject name
     */
    void setName(const String& name);

    /**
     * @brief Gets the GameObject name
     */
    const String& getName() const;

    /**
     * @brief Gets the parent GameObject.
     */
    GameObject* getParent() const;

    /**
     * @brief Adds a child GameObject. The parent will take ownership of the
     * child.
     */
    void addChild(UniquePtr<GameObject> child);

    /**
     * @brief Removes a child GameObject. Returns ownership to the caller if
     * found, nullptr otherwise.
     */
    UniquePtr<GameObject> removeChild(GameObject* child);

    /**
     * @brief Gets the children of this GameObject.
     */
    const Vector<UniquePtr<GameObject>>& getChildren() const;

    /**
     * @brief Gets the world transformation matrix. Combines this object's
     * transform with all ancestors.
     */
    Matrix4 getWorldMatrix() const;

    /**
     * @brief Adds a component to this GameObject. The GameObject takes
     * ownership of the component.
     */
    void addComponent(UniquePtr<IComponent> component);

    /**
     * @brief Gets the components attached to this GameObject.
     */
    const Vector<UniquePtr<IComponent>>& getComponents() const;

  private:
    String m_name;
    GameObject* m_parent = nullptr;
    Vector<UniquePtr<GameObject>> m_children;
    Vector<UniquePtr<IComponent>> m_components;
    Vector<IDrawable*> m_drawableComponents;
  };
}
