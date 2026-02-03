#pragma once

#include "hc/hcABaseComponent.h"

namespace hc
{
  class Camera;
  class CameraManager;

  class HC_CORE_EXPORT CameraComponent : public ABaseComponent
  {
  public:
    CameraComponent();
    ~CameraComponent() override;

    void setAsActiveCamera();

  private:
    Camera* m_camera;

    CameraManager& getCameraManager();
  };
}
