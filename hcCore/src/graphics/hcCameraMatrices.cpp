#include "hc/graphics/hcCameraMatrices.h"
#include "hc/scene/camera/hcCamera.h"

namespace hc
{
  CameraRenderData CameraRenderData::Create(Camera& camera)
  {
    CameraRenderData cameraRenderData;
    cameraRenderData.projectionMatrix = camera.getProjectionMatrix();
    cameraRenderData.viewMatrix = camera.getViewMatrix();
    cameraRenderData.cameraWorldPosition = camera.getPosition();
    return cameraRenderData;
  }

  CameraRenderData CameraRenderData::Create(const Camera& camera)
  {
    CameraRenderData cameraRenderData;
    cameraRenderData.projectionMatrix = camera.getCachedProjectionMatrix();
    cameraRenderData.viewMatrix = camera.getViewMatrix();
    cameraRenderData.cameraWorldPosition = camera.getPosition();
    return cameraRenderData;
  }
}
