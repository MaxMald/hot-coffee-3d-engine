#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class CameraManagerWindow : public AWindowView
  {
  public:
    CameraManagerWindow(SceneManager& sceneManager);
    ~CameraManagerWindow();

    void destroy() override;

  private:
    SceneManager& m_sceneManager;
    Vector<Camera*> m_cameras;

    void onDraw() override;
    void drawCameraController(Camera* camera);
    void drawCameraProjectionController(ICameraProjection* projection);
    void drawPerspectiveProjectionController(PerspectiveCameraProjection* projection);
    void drawOrthographicProjectionController(OrthographicCameraProjection* projection);
  };
}
