#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class CameraManagerWindow :
    public AWindowView,
    public IDependencyResolvable
  {
  public:
    CameraManagerWindow();
    ~CameraManagerWindow();

    void resolveDependencies(DependencyContainer& container) override;

  private:
    SceneManager* m_sceneManager;

    void onDraw() override;
    void drawCameraController(Camera* camera);
  };
}
