#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/camera/hcCameraManager.h"
#include "hc/graphics/lightFrameData/hcLightFrameData.h"

namespace hc
{
  class IGameObjectFactory;
  class SceneManager;
  class IGraphicsManager;

  /**
   * @brief Represents a 3D scene containing game objects, lights, and cameras.
   *
   * The Scene class manages the lifecycle and organization of game objects,
   * lights, and cameras. It provides methods for creating game objects, managing
   * the scene graph, and handling scene activation, deactivation, drawing, and
   * updating. The class is intended to be subclassed for custom scene logic by
   * overriding the protected virtual hooks.
   */
  class HC_CORE_EXPORT Scene : public NonCopyable, public ISerializable
  {
  public:
    /**
     * @brief Constructs a Scene instance.
     */
    Scene();
    virtual ~Scene();

    /**
     * @brief Serializes the scene's light manager, camera manager, and scene
     * graph to a binary writer.
     *
     * Writes all scene data including lights, cameras, and the entire scene
     * graph hierarchy to the binary stream.
     *
     * @param writer The BinaryWriter to serialize to.
     */
    void serialize(BinaryWriter& writer) const override;

    /**
     * @brief Deserializes the scene's light manager, camera manager, and scene
     * graph from a binary reader.
     *
     * Clears existing scene data and reads all scene data including lights,
     * cameras, and the entire scene graph hierarchy from the binary stream.
     *
     * @param reader The BinaryReader to deserialize from.
     */
    void deserialize(BinaryReader& reader) override;

    /**
     * @brief Creates a new GameObject with the specified name.
     *
     * @param name The name to assign to the created GameObject.
     *
     * @throws RuntimeErrorException if the GameObjectFactory is not initialized.
     * @return UniquePtr<GameObject> The created GameObject.
     */
    UniquePtr<GameObject> createGameObject(const String& name);

    /**
     * @brief Creates and adds a new root GameObject to the scene graph.
     *
     * @param name The name to assign to the created root GameObject.
     *
     * @throws RuntimeErrorException if GameObject creation fails.
     * @return Pointer to the created root GameObject.
     */
    GameObject* createRootGameObject(const String& name);

    /**
     * @brief Gets a reference to the scene graph for modification.
     *
     * @return Reference to the SceneGraph.
     */
    SceneGraph& getSceneGraph();

    /**
     * @brief Gets a const reference to the scene graph.
     *
     * @return Const reference to the SceneGraph.
     */
    const SceneGraph& getSceneGraph() const;

    /**
     * @brief Gets a reference to the camera manager.
     *
     * @return Reference to the CameraManager.
     */
    CameraManager& getCameraManager();

    /**
     * @brief Gets a const reference to the camera manager.
     *
     * @return Const reference to the CameraManager.
     */
    const CameraManager& getCameraManager() const;

    /**
     * @brief Draws the scene using the active camera. If no active camera is set,
     * the default camera will be used.
     * 
     * @param graphicsManager The graphics manager to use for drawing the scene.
     */
    void draw(IGraphicsManager& graphicsManager);

    /**
     * @brief Draws the scene using the specified camera.
     *
     * @param graphicsManager The graphics manager to use for drawing the scene.
     * @param camera The camera to use for drawing the scene.
     */
    void draw(IGraphicsManager& graphicsManager, Camera* camera);

    /**
     * @brief Clears the scene graph, lights, and cameras.
     */
    void clear();

  protected:
    /**
     * @brief Called after the scene is initialized.
     *
     * Override to perform custom initialization logic.
     */
    virtual void onInitialized();

    /**
     * @brief Called when the scene is activated.
     *
     * Override to perform custom activation logic.
     */
    virtual void onActivate();

    /**
     * @brief Called when the scene is deactivated.
     *
     * Override to perform custom deactivation logic.
     */
    virtual void onDeactivate();

    /**
     * @brief Called to load resources when the scene is activated.
     *
     * Override to load custom resources.
     */
    virtual void onLoad();

    /**
     * @brief Called after loading resources and before the first update/draw.
     *
     * Override to perform preparation logic.
     */
    virtual void onPrepare();

    /**
     * @brief Called before the scene graph is drawn.
     *
     * Override to perform actions before drawing.
     *
     * @param renderContext The current render context.
     */
    virtual void onBeforeDraw(const RenderContext& renderContext);

    /**
     * @brief Called after the scene graph is drawn.
     *
     * Override to perform actions after drawing.
     *
     * @param renderContext The current render context.
     */
    virtual void onAfterDraw(const RenderContext& renderContext);

    /**
     * @brief Called before the scene graph is updated.
     *
     * Override to perform actions before updating.
     * 
     * @param elapsedTime The elapsed time since the last update.
     */
    virtual void onBeforeUpdate(const Time& elapsedTime);

    /**
     * @brief Called after the scene graph is updated.
     *
     * Override to perform actions after updating.
     *
     * @param elapsedTime The elapsed time since the last update.
     */
    virtual void onAfterUpdate(const Time& elapsedTime);

    /**
     * @brief Called when the scene is destroyed.
     *
     * Override to perform cleanup logic.
     */
    virtual void onDestroy();

    /**
     * @brief Called during serialization to write custom scene data.
     *
     * Override to serialize additional custom data for derived scene classes.
     * Called after the base scene data (lights, cameras, scene graph) is
     * serialized.
     *
     * @param writer The BinaryWriter to serialize custom data to.
     */
    virtual void onSerialize(BinaryWriter& writer) const;

    /**
     * @brief Called during deserialization to read custom scene data.
     *
     * Override to deserialize additional custom data for derived scene
     * classes. Called after the base scene data (lights, cameras, scene
     * graph) is deserialized.
     *
     * @param reader The BinaryReader to deserialize custom data from.
     */
    virtual void onDeserialize(BinaryReader& reader);

  private:
    SceneGraph m_sceneGraph;
    CameraManager m_cameraManager;
    LightFrameData m_lightFrameData;
    IGameObjectFactory* m_gameObjectFactory;

    /**
     * @brief Initializes the scene with a GameObjectFactory.
     *
     * @param gameObjectFactory Pointer to the factory to use.
     * @throws InvalidArgumentException if the pointer is null.
     */
    void initialize(IGameObjectFactory* gameObjectFactory);

    /**
     * @brief Activates the scene, calling onActivate(), onLoad(), and onPrepare() hooks.
     */
    void activate();

    /**
     * @brief Deactivates the scene.
     */
    void deactivate();

    /**
     * @brief Updates the scene and its game objects.
     *
     * @param elapsedTime The elapsed time since the last update.
     */
    void update(const Time& elapsedTime);

    /**
     * @brief Destroys the scene, performing cleanup and clearing contents.
     */
    void destroy();

    friend class SceneManager;
  };
}
