#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcAWindowView.h"

namespace hc
{
  class SceneManager;
  class Light;
}

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
    SharedPtr<SceneManager> m_sceneManager;

    void onDraw() override;
    void drawLightController(Light* light);
  };
}
