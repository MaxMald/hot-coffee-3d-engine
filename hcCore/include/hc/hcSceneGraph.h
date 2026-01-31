#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIDrawable.h"
#include "hc/hcGameObject.h"

namespace hc
{
  /**
   * @brief Manages a collection of root GameObjects, organized by key
   * (layer/group).
   *
   * The SceneGraph allows for flexible scene organization by supporting multiple
   * root objects, each identified by a unique key (such as a layer or group
   * name). It is responsible for updating and rendering all root objects and
   * their hierarchies.
   */
  class HC_CORE_EXPORT SceneGraph :
    public NonCopyable,
    public IDrawable
  {
  public:
    /**
     * @brief Constructs an empty SceneGraph.
     */
    SceneGraph();

    /**
     * @brief Virtual destructor.
     */
    virtual ~SceneGraph();

    /**
     * @brief Renders all root GameObjects and their children.
     *
     * @param graphicsManager The graphics manager used for rendering.
     */
    virtual void draw(IGraphicsManager& graphicsManager) override;

    /**
     * @brief Updates all root GameObjects and their children.
     *
     * @param deltaTime Time elapsed since last frame (in seconds).
     */
    void update(float deltaTime);

    /**
     * @brief Adds a root GameObject for the given key. SceneGraph takes
     * ownership of the object.
     *
     * @param key The unique key identifying the root (e.g., layer name).
     * @param root The GameObject to add as a root.
     */
    void setRoot(const String& key, UniquePtr<GameObject> root);

    /**
     * @brief Removes a root GameObject by key. Returns ownership to the caller
     * if found, nullptr otherwise.
     *
     * @param key The key of the root to remove.
     * 
     * @return The removed GameObject, or nullptr if not found.
     */
    UniquePtr<GameObject> removeRoot(const String& key);

    /**
     * @brief Gets a root GameObject by key.
     * 
     * @param key The key of the root to retrieve.
     * 
     * @return Pointer to the GameObject, or nullptr if not found.
     */
    GameObject* getRoot(const String& key) const;

    /**
     * @brief Gets a const reference to all root GameObjects mapped by their
     * keys.
     *
     * @return Const reference to the internal map of roots.
     */
    const UnorderedMap<String, UniquePtr<GameObject>>& getRoots() const;

  private :
    /**
     * @brief Map of root GameObjects, keyed by unique string (layer/group name).
     */
    UnorderedMap<String, UniquePtr<GameObject>> m_roots;
  };
}
