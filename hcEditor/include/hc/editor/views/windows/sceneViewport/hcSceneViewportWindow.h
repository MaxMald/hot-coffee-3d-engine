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

    void onDraw() override;
    void onWindowSizeChanged(const Vector2f& newSize) override;

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
