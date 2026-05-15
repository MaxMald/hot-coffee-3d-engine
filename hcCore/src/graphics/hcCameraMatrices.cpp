#include "hc/graphics/hcCameraMatrices.h"
#include "hc/scene/camera/hcCamera.h"

namespace hc
{
  CameraMatrices CameraMatrices::Create(Camera& camera)
  {
    CameraMatrices cameraMatrices;
    cameraMatrices.projectionMatrix = camera.getProjectionMatrix();
    cameraMatrices.viewMatrix = camera.getViewMatrix();
    return cameraMatrices;
  }

  CameraMatrices CameraMatrices::Create(const Camera& camera)
  {
    CameraMatrices cameraMatrices;
    cameraMatrices.projectionMatrix = camera.getCachedProjectionMatrix();
    cameraMatrices.viewMatrix = camera.getViewMatrix();
    return cameraMatrices;
  }
}
