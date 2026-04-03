#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class SceneViewportWindow : public AWindowView
  {
  public:
    SceneViewportWindow(HotCoffeeEngine& engine);

  protected:
    HotCoffeeEngine& m_engine;
    FrameBufferPtr m_frameBuffer;
    Vector2f m_uvTopLeft;
    Vector2f m_uvBottomRight;

    void onDraw() override;

    /**
     * @brief Updates framebuffer size to match content region.
     */
    void updateFramebufferSize();

    /**
     * @brief Renders the content scene to the render texture.
     */
    void renderSceneToTexture();

    /**
     * @brief Draws the viewport window with the rendered scene texture.
     */
    void drawViewport();
  };
}
