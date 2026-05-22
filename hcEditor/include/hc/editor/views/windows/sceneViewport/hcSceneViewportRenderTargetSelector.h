#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/windows/sceneViewport/hcSceneViewportRenderTargetType.h"

namespace hc::editor
{
  class SceneViewportRenderTargetSelector
  {
  public:
    /**
     * @brief Constructs a SceneViewportRenderTargetSelector.
     *
     * @param initialRenderTarget The initial render target type.
     * @param graphicsManager The graphics manager instance.
     */
    SceneViewportRenderTargetSelector(
      sceneViewportRenderTargetType::Type initialRenderTarget,
      IGraphicsManager& graphicsManager
    );

    ~SceneViewportRenderTargetSelector();

    /**
     * @brief Gets the currently selected render target type.
     *
     * @return The current render target type.
     */
    sceneViewportRenderTargetType::Type getCurrentRenderTarget() const;

    /**
     * @brief Draws a combo box for selecting the render target type.
     */
    void draw();

  private:
    sceneViewportRenderTargetType::Type m_currentRenderTarget;
    IGraphicsManager& m_graphicsManager;
  };
}
