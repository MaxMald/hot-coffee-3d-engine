#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcTransform.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIDrawable.h"

namespace hc
{
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
     * @brief Renders the game object and its children.
     */
    virtual void draw(IGraphicsManager& graphicsManager) override;

    /**
     * @brief Updates the game object and its children.
     * 
     * @param deltaTime Time elapsed since last frame (in seconds).
     */
    void update(float deltaTime);

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

  private:
    String m_name;
    GameObject* m_parent = nullptr;
    Vector<UniquePtr<GameObject>> m_children;
  };
}
