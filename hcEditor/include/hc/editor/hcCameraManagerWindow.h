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
    void onDraw() override;
    void drawCameraController(Camera* camera);
    void drawCameraProjectionController(ICameraProjection* projection);
    void drawPerspectiveProjectionController(PerspectiveCameraProjection* projection);
    void drawOrthographicProjectionController(OrthographicCameraProjection* projection);
  };
}
