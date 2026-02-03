#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class CameraManagerWindow : public AWindowView
  {
  public:
    CameraManagerWindow();
    ~CameraManagerWindow();

  private:
    SceneManager* m_sceneManager;

    void onDraw() override;
    void drawCameraController(Camera* camera);
  };
}
