#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class SceneViewportWindow;

  class SceneViewportSettingsWindow : public AWindowView
  {
  public:
    SceneViewportSettingsWindow(
      SceneViewportWindow& viewportWindow,
      IGraphicsManager& graphicsManager
    );
    virtual ~SceneViewportSettingsWindow();

    void destroy() override;
  private:
    SceneViewportWindow& m_viewportWindow;
    IGraphicsManager& m_graphicsManager;

    void onDraw() override;
    void drawViewportSettings(SceneViewportWindow& viewportWindow);
  };
}
