#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcSceneGraph.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIDrawable.h"

namespace hc
{
  class SceneManager;

  /**
   * @brief Represents a scene containing a scene graph and providing update and
   * draw functionality.
   */
  class Scene :
    public NonCopyable,
    public IDrawable
  {
  public:
    Scene();
    virtual ~Scene();

    /**
     * @brief Renders the entire scene using the provided graphics manager.
     *
     * @param graphicsManager The graphics manager used for rendering.
     */
    virtual void draw(IGraphicsManager& graphicsManager) override;

    /**
     * @brief Updates the scene and all contained game objects.
     * 
     * @param deltaTime Time elapsed since last frame (in seconds).
     */
    void update(float deltaTime);

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

  private:
    SceneGraph m_sceneGraph;

    /**
     * @brief Called when the scene becomes active.
     */
    void onActivate();

    /**
     * @brief Called when the scene becomes inactive.
     */
    void onDeactivate();

    /**
     * @brief Called when the scene is being destroyed.
     */
    void onDestroy();

    friend class SceneManager;
  };
}
