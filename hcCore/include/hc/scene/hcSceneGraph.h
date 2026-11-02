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
    public io::ISerializable,
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
    void serialize(io::BinaryWriter& writer) const override;

    /**
     * @brief Deserializes the SceneGraph and all root GameObjects from binary format.
     *
     * @param reader The BinaryReader to use for deserialization.
     */
    void deserialize(io::BinaryReader& reader) override;

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
     * @brief Gets the root GameObject of the scene graph.
     *
     * @return Pointer to the root GameObject.
     */
    inline GameObject* getRoot() const
    {
      return m_root.get();
    }

    /**
     * @brief Adds a GameObject to the root level of the scene graph.
     *
     * @param gameObject Unique pointer to the root GameObject to add.
     */
    void addGameObject(UniquePtr<GameObject> gameObject);

    /**
     * @brief Removes the first found GameObject with the specified name from the scene
     * graph and returns ownership of it.
     *
     * @note This method is recursive and will search through all GameObjects and their
     * descendants to find a match.
     *
     * @param name The name of the GameObject to remove.
     *
     * @return Unique pointer to the removed GameObject, or nullptr if not found.
     */
    UniquePtr<GameObject> removeGameObject(const String& name);

    /**
     * @brief Removes the GameObject with the specified UUID from the scene graph and
     * returns ownership of it.
     *
     * @note This method is recursive and will search through all GameObjects and their
     * descendants to find a match.
     *
     * @param uuid The UUID of the GameObject to remove.
     *
     * @return Unique pointer to the removed GameObject, or nullptr if not found.
     */
    UniquePtr<GameObject> removeGameObject(const UUID& uuid);

    /**
     * @brief Gets the first found GameObject with the specified name.
     *
     * @note This method is recursive and will search through all GameObjects and their
     * descendants to find a match.
     *
     * @param name The name of the GameObject to find.
     *
     * @return Pointer to the found GameObject, or nullptr if not found.
     */
    GameObject* getGameObject(const String& name) const;

    /**
     * @brief Gets the GameObject with the specified UUID.
     *
     * @note This method is recursive and will search through all GameObjects and their
     * descendants to find a match.
     *
     * @param uuid The UUID of the GameObject to find.
     *
     * @return Pointer to the found GameObject, or nullptr if not found.
     */
    GameObject* getGameObject(const UUID& uuid) const;

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
     * @brief Root GameObject of the scene graph. All other GameObjects are descendants of
     * this root.
     */
    UniquePtr<GameObject> m_root;

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

    /**
     * @brief Asserts that the SceneGraph is initialized and throws an exception if not.
     *
     * @throw RuntimeErrorException If the SceneGraph is not initialized.
     */
    inline void assertIsInitialized() const
    {
      if (m_root == nullptr)
        throw RuntimeErrorException("Root GameObject is not initialized. Cannot perform operation.");
      if (m_gameObjectFactory == nullptr)
        throw RuntimeErrorException("GameObjectFactory is not initialized. Cannot perform operation.");
    }

    friend class Scene;
  };
}
