#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class LightManagerWindow :
    public AWindowView,
    public IDependencyResolvable
  {
  public:
    LightManagerWindow();
    ~LightManagerWindow();

    void resolveDependencies(DependencyContainer& container) override;

  protected:
    SceneManager* m_sceneManager;

    void onDraw() override;
    void drawLightController(Light* light);
  };
}
