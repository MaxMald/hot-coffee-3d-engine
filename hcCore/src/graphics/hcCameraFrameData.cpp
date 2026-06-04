#include "hc/graphics/hcCameraFrameData.h"
#include "hc/scene/camera/hcCamera.h"

namespace hc
{
  CameraFrameData  CameraFrameData::Create(Camera& camera)
  {
    CameraFrameData cameraFrameData;
    cameraFrameData.projectionMatrix = camera.getProjectionMatrix();
    cameraFrameData.viewMatrix = camera.getViewMatrix();
    cameraFrameData.cameraWorldPosition = camera.getPosition();
    return cameraFrameData;
  }

  CameraFrameData CameraFrameData::Create(const Camera& camera)
  {
    CameraFrameData cameraFrameData;
    cameraFrameData.projectionMatrix = camera.getCachedProjectionMatrix();
    cameraFrameData.viewMatrix = camera.getViewMatrix();
    cameraFrameData.cameraWorldPosition = camera.getPosition();
    return cameraFrameData;
  }
}
