#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcSceneGraph.h"
#include "hc/hcLightManager.h"
#include "hc/hcCameraManager.h"

namespace hc
{
  class SceneManager;
  class IGameObjectFactory;

  /**
   * @brief Represents a scene containing all objects, lights, and cameras, and
   * provides update and draw functionality.
   *
   * The Scene class manages the lifecycle and organization of all game objects,
   * lights, and cameras within a scene. It owns a scene graph for hierarchical
   * object management, and provides access to light and camera managers.
   */
  class HC_CORE_EXPORT Scene : public NonCopyable
  {
  public:
    /**
     * @brief Constructs a Scene with the provided game object factory.
     * 
     * @param gameObjectFactory Reference to the game object factory used for
     * creating game objects.
     */
    Scene(IGameObjectFactory& gameObjectFactory);
    virtual ~Scene();

    /**
     * @brief Creates a new GameObject with the specified name.
     * 
     * @param name The name to assign to the created GameObject.
     * 
     * @return UniquePtr<GameObject> The created GameObject.
     */
    UniquePtr<GameObject> createGameObject(const String& name) const;

    /**
     * @brief Creates a new root GameObject with the specified name and adds it
     * to the scene graph.
     * 
     * @param name The name to assign to the created root GameObject.
     * 
     * @return Pointer to the created root GameObject.
     */
    GameObject* createRootGameObject(const String& name);

    /**
     * @brief Renders the entire scene using the provided graphics manager.
     */
    void draw();

    /**
     * @brief Updates the scene and all contained game objects.
     * 
     * @param elapsedTime Time elapsed since last frame.
     */
    void update(const Time& elapsedTime);

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
     * @brief Gets a reference to the light manager for managing lights in the
     * scene.
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
     * @brief Gets a reference to the camera manager for managing cameras in the
     * scene.
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

  private:
    IGameObjectFactory& m_gameObjectFactory;
    SceneGraph m_sceneGraph;
    LightManager m_lightManager;
    CameraManager m_cameraManager;

    /**
     * @brief Called when the scene becomes active. Intended for internal use by
     * SceneManager.
     */
    void onActivate();

    /**
     * @brief Called when the scene becomes inactive. Intended for internal use
     * by SceneManager.
     */
    void onDeactivate();

    /**
     * @brief Called when the scene is being destroyed. Intended for internal use
     * by SceneManager.
     */
    void onDestroy();

    friend class SceneManager;
  };
}
