#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcGameObject.h"
#include "hc/hcIDrawable.h"

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
    virtual ~SceneGraph() override;

    /**
     * @brief Renders all root GameObjects and their children.
     *
     * @param graphicsManager The graphics manager used for rendering.
     */
    void draw(const RenderContext& renderContext);

    /**
     * @brief Updates all root GameObjects and their children.
     *
     * @param elapsedTime Time elapsed since last frame.
     */
    void update(const Time& elapsedTime);

    void addRoot(UniquePtr<GameObject> root);

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
    const Vector<UniquePtr<GameObject>>& getRoots() const;

  private :
    /**
     * @brief Map of root GameObjects, keyed by unique string (layer/group name).
     */
    Vector<UniquePtr<GameObject>> m_roots;
  };
}
