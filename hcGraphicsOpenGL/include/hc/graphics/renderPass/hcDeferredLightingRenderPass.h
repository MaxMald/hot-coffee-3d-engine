#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGBuffer;
  class OpenGlLightShadowMapManager;

  class DeferredLightingRenderPass
  {
  public:
    DeferredLightingRenderPass(OpenGlLightShadowMapManager& lightShadowMapManager);
    ~DeferredLightingRenderPass();

    void initialize(
      OpenGlGBuffer* gBuffer,
      SharedPtr<IShaderProgram> deferredLightingShaderProgram
    );
    void execute(IFrameBuffer* currentRenderTarget);
    void destroy();

  private:
    OpenGlGBuffer* m_gBuffer;
    OpenGlLightShadowMapManager& m_lightShadowMapManager;
    SharedPtr<IShaderProgram> m_deferredLightingShaderProgram;
  };
}
