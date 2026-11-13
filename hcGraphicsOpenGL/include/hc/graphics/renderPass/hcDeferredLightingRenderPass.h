#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc::graphics::shadows
{
  class OpenGlLightShadowMapManager;
}

namespace hc
{
  class OpenGlGBuffer;

  class DeferredLightingRenderPass
  {
  public:
    DeferredLightingRenderPass(graphics::shadows::OpenGlLightShadowMapManager& lightShadowMapManager);
    ~DeferredLightingRenderPass();

    void initialize(
      OpenGlGBuffer* gBuffer,
      SharedPtr<IShaderProgram> deferredLightingShaderProgram
    );
    void execute(IFrameBuffer* currentRenderTarget);
    void destroy();

  private:
    OpenGlGBuffer* m_gBuffer;
    graphics::shadows::OpenGlLightShadowMapManager& m_lightShadowMapManager;
    SharedPtr<IShaderProgram> m_deferredLightingShaderProgram;
  };
}
