#include "hc/graphics/hcRenderContext.h"
#include "hc/scene/camera/hcCamera.h"

namespace hc
{
  RenderContext RenderContext::Create(
    Camera& camera,
    const Matrix4& modelTransform
  )
  {
    RenderContext renderContext;
    renderContext.cameraMatrices = CameraMatrices::Create(camera);
    renderContext.cameraPosition = camera.getPosition();
    renderContext.transform = modelTransform;
    renderContext.modelPosition = Matrix4::ExtractTranslation(modelTransform);
    return renderContext;
  }
}
