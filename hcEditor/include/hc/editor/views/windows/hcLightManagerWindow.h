#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class LightManagerWindow : public AWindowView
  {
  public:
    LightManagerWindow(SceneManager& sceneManager);
    ~LightManagerWindow();

    void destroy() override;

  protected:
    SceneManager& m_sceneManager;
    Vector<Light*> m_lights;

    void onDraw() override;
    void drawLightController(Light* light);
  };
}
