#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/gameObject/hcGameObject.h"
#include "hc/graphics/hcIDrawable.h"

namespace hc
{
  class IGameObjectFactory;
  class Scene;
  struct DrawCommand;

  /**
   * @brief Organizes and manages root-level GameObjects in the scene.
   *
   * The SceneGraph is responsible for maintaining a collection of root
   * GameObjects, each representing a separate hierarchy (such as a layer or
   * group) in the scene. It provides methods for adding, removing, updating, and
   * rendering these root objects and their entire subtrees. Ownership of root
   * GameObjects is managed by the SceneGraph.
   *
   * @note SceneGraph is non-copyable.
   */
  class HC_CORE_EXPORT SceneGraph :
    public NonCopyable,
    public ISerializable,
    public IDrawable
  {
  public:
    /**
     * @brief Constructs an empty SceneGraph.
     */
    SceneGraph();

    /**
     * @brief Destroys the SceneGraph and all root GameObjects it owns.
     */
    virtual ~SceneGraph() override;

    /**
     * @brief Serializes the SceneGraph and all root GameObjects to binary format.
     *
     * @param writer The BinaryWriter to use for serialization.
     */
    void serialize(BinaryWriter& writer) const override;

    /**
     * @brief Deserializes the SceneGraph and all root GameObjects from binary format.
     *
     * @param reader The BinaryReader to use for deserialization.
     */
    void deserialize(BinaryReader& reader) override;

    /**
     * @brief Renders all root GameObjects and their hierarchies, populating the
     * provided vector with draw commands.
     *
     * @param renderContext The rendering context to use.
     * @param outDrawCommands Vector to populate with draw commands.
     */
    void draw(
      const RenderContext& renderContext,
      Vector<DrawCommand>& outDrawCommands
    ) const;

    /**
     * @brief Updates all root GameObjects and their hierarchies.
     *
     * @param elapsedTime Time elapsed since the last update.
     */
    void update(const Time& elapsedTime);

    /**
     * @brief Adds a root GameObject to the scene graph.
     *
     * The GameObject must not have a parent. Ownership is transferred to the
     * SceneGraph.
     *
     * @param root Unique pointer to the root GameObject to add.
     */
    void addRoot(UniquePtr<GameObject> root);

    /**
     * @brief Removes the first root GameObject with the specified name from the
     * scene graph and returns ownership to the caller.
     *
     * @param name The name of the root GameObject to remove.
     *
     * @return Unique pointer to the removed GameObject, or nullptr if not found.
     */
    UniquePtr<GameObject> removeRoot(const String& name);

    /**
     * @brief Retrieves the first root GameObject with the specified name.
     *
     * @param name The name of the root GameObject to retrieve.
     *
     * @return Pointer to the GameObject, or nullptr if not found.
     */
    GameObject* getRoot(const String& name) const;

    /**
     * @brief Gets a const reference to all root GameObjects.
     * 
     * @return Const reference to the vector of root GameObjects.
     */
    const Vector<UniquePtr<GameObject>>& getRoots() const;

    /**
     * @brief Populates the provided vector with pointers to all GameObjects in the
     * scene graph, including all root objects and their descendants.
     *
     * @param outGameObjects Vector to populate with GameObject pointers.
     */
    void getAllGameObjects(Vector<GameObject*>& outGameObjects) const;

    /**
     * @brief Removes and destroys all root GameObjects from the scene graph.
     */
    void clear();

  private:
    /**
     * @brief Collection of root GameObjects owned by the SceneGraph.
     */
    Vector<UniquePtr<GameObject>> m_roots;

    /**
     * @brief Pointer to the GameObjectFactory used for creating GameObjects during
     * deserialization.
     */
    IGameObjectFactory* m_gameObjectFactory;

    /**
     * @brief Initializes the SceneGraph with a GameObjectFactory for deserialization.
     *
     * @param gameObjectFactory Pointer to the factory to use for creating GameObjects.
     */
    void initialize(IGameObjectFactory* gameObjectFactory);

    friend class Scene;
  };
}
