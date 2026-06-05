#include "hc/graphics/hcRenderContext.h"
#include "hc/scene/camera/hcCamera.h"

namespace hc
{
  RenderContext RenderContext::Create(Camera& camera, const Matrix4& modelTransform)
  {
    RenderContext renderContext;
    renderContext.transform = modelTransform;
    renderContext.modelPosition = Matrix4::ExtractTranslation(modelTransform);
    renderContext.cameraWorldPosition = camera.getPosition();
    return renderContext;
  }

  RenderContext RenderContext::Create(const Camera& camera, const Matrix4& modelTransform)
  {
    RenderContext renderContext;
    renderContext.transform = modelTransform;
    renderContext.modelPosition = Matrix4::ExtractTranslation(modelTransform);
    renderContext.cameraWorldPosition = camera.getPosition();
    return renderContext;
  }
}
