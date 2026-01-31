#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcScene.h"
#include "hc/hcIDrawable.h"

namespace hc
{
  /**
   * @brief Manages multiple scenes and controls the active scene.
   */
  class HC_CORE_EXPORT SceneManager :
    public NonCopyable,
    public IDrawable
  {
  public:
    SceneManager();
    ~SceneManager();

    /**
     * @brief Draws the active scene.
     *
     * @param graphicsManager The graphics manager used for rendering.
     */
    virtual void draw(IGraphicsManager& graphicsManager) override;

    /**
     * @brief Creates and adds a new scene with the given name. If a scene with
     * the same name exists, it is replaced.
     * 
     * @param name The unique name for the scene.
     * 
     * @return Pointer to the created Scene.
     */
    Scene* createScene(const String& name);

    /**
     * @brief Removes a scene by name.
     * 
     * @param name The name of the scene to remove.
     * 
     * @return True if the scene was removed, false if not found.
     */
    bool removeScene(const String& name);

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
     * @param deltaTime Time elapsed since last frame (in seconds).
     */
    void update(float deltaTime);

  private:
    UnorderedMap<String, UniquePtr<Scene>> m_scenes;
    Scene* m_activeScene;
  };
}
