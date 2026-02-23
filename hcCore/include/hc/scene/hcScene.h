#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/light/hcLightManager.h"
#include "hc/scene/camera/hcCameraManager.h"

namespace hc
{
  class IGameObjectFactory;
  class SceneManager;

  /**
   * @brief Represents a 3D scene containing game objects, lights, and cameras.
   *
   * The Scene class manages the lifecycle and organization of game objects,
   * lights, and cameras. It provides methods for creating game objects, managing
   * the scene graph, and handling scene activation, deactivation, drawing, and
   * updating. The class is intended to be subclassed for custom scene logic by
   * overriding the protected virtual hooks.
   */
  class HC_CORE_EXPORT Scene : public NonCopyable
  {
  public:
    /**
     * @brief Constructs a Scene instance.
     */
    Scene();

    virtual ~Scene();

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
     * @brief Gets a reference to the light manager.
     *
     * @return Reference to the LightManager.
     */
    LightManager& getLightManager();

    /**
     * @brief Gets a const reference to the light manager.
     *
     * @return Const reference to the LightManager.
     */
    const LightManager& getLightManager() const;

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
     * @brief Clears the scene graph, lights, and cameras.
     */
    void clear();

  private:
    SceneGraph m_sceneGraph;
    LightManager m_lightManager;
    CameraManager m_cameraManager;
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
     * @brief Draws the scene using the active or default camera.
     */
    void draw();

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
