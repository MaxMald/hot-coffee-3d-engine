#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/hcScene.h"

namespace hc
{
  class IGameObjectFactory;

  /**
   * @brief Manages multiple scenes and controls the active scene.
   *
   * This class is non-copyable.
   */
  class HC_CORE_EXPORT SceneManager : public NonCopyable
  {
  public:
    SceneManager();
    ~SceneManager();

    /**
     * @brief Initializes the scene manager with a game object factory.
     *
     * @param gameObjectFactory Unique pointer to the IGameObjectFactory to use
     * for creating game objects in scenes.
     */
    void initialize(UniquePtr<IGameObjectFactory> gameObjectFactory);

    /**
     * @brief Draws the active scene.
     *
     * If no scene is active, this method does nothing.
     */
    void draw();

    /**
     * @brief Creates a new scene of the specified type and adds it to the
     * manager.
     *
     * The scene is constructed with the provided arguments. If a scene with the
     * same name exists, a RuntimeErrorException is thrown. The new scene is
     * added to the manager and can be retrieved or set as active by name.
     *
     * @tparam SceneType The type of the scene to create (must derive from
     * Scene).
     * @tparam TArgs Constructor argument types for the scene.
     *
     * @param name The unique name for the new scene.
     * @param args Arguments to forward to the scene's constructor.
     *
     * @return Pointer to the newly created scene of the specified type.
     *
     * @throws RuntimeErrorException if a scene with the given name already
     * exists.
     */
    template<typename SceneType, typename... TArgs>
    SceneType* createScene(const String& name, TArgs&&... args);

    /**
     * @brief Removes a scene by name.
     *
     * @param name The name of the scene to remove.
     *
     * @return True if the scene was removed, false if not found.
     */
    bool removeScene(const String& name);

    /**
     * @brief Checks if a scene with the given name exists.
     *
     * @param name The name of the scene to check.
     *
     * @return True if the scene exists, false otherwise.
     */
    bool hasScene(const String& name) const;

    /**
     * @brief Retrieves a scene by name.
     *
     * @param name The name of the scene.
     *
     * @return Pointer to the Scene, or nullptr if not found.
     */
    Scene* getScene(const String& name) const;

    /**
     * @brief Sets the active scene by name.
     *
     * @param name The name of the scene to activate.
     *
     * @return True if the scene was set as active, false if not found.
     */
    bool setActiveScene(const String& name);

    /**
     * @brief Gets the currently active scene.
     *
     * @return Pointer to the active Scene, or nullptr if none is active.
     */
    Scene* getActiveScene() const;

    /**
     * @brief Updates the active scene.
     *
     * @param elapsedTime Time elapsed since last frame.
     *
     * If no scene is active, this method does nothing.
     */
    void update(const Time& elapsedTime);

    /**
     * @brief Destroys all scenes and clears the scene manager.
     *
     * After calling this, no scenes remain in the manager.
     */
    void clear();

    /**
     * @brief Destroys the scene manager and all managed scenes.
     *
     * This is equivalent to calling clear().
     */
    void destroy();

  private:
    UnorderedMap<String, UniquePtr<Scene>> m_scenes;
    Scene* m_activeScene;
    UniquePtr<IGameObjectFactory> m_gameObjectFactory;

    /**
     * @brief Adds a scene to the manager with the given name.
     *
     * @param name The name of the scene.
     * @param scene The unique pointer to the scene instance.
     */
    void addScene(const String& name, UniquePtr<Scene> scene);
  };

  template<typename SceneType, typename... TArgs>
  SceneType* SceneManager::createScene(const String& name, TArgs&&... args)
  {
    static_assert(
      std::derived_from<SceneType, Scene>,
      "SceneType must derive from Scene");

    if (hasScene(name))
    {
      throw RuntimeErrorException(
        String::Format("Scene with name '%s' already exists", name.c_str())
      );
    }

    UniquePtr<SceneType> newScene =
      MakeUnique<SceneType>(std::forward<TArgs>(args)...);

    SceneType* scenePtr = newScene.get();
    addScene(name, std::move(newScene));
    return scenePtr;
  }
}
