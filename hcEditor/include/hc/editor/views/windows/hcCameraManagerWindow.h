#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class CameraManagerWindow : public AWindowView
  {
  public:
    CameraManagerWindow(SceneManager& sceneManager);
    ~CameraManagerWindow();

  private:
    SceneManager& m_sceneManager;

    void onDraw() override;
    void drawCameraController(Camera* camera);
    void drawCameraProjectionController(ICameraProjection* projection);
    void drawPerspectiveProjectionController(PerspectiveCameraProjection* projection);
    void drawOrthographicProjectionController(OrthographicCameraProjection* projection);
  };
}
