#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGBuffer;
  class OpenGlLightShadowManager;

  class DeferredLightingRenderPass
  {
  public:
    DeferredLightingRenderPass(OpenGlLightShadowManager& lightShadowManager);
    ~DeferredLightingRenderPass();

    void initialize(
      OpenGlGBuffer* gBuffer,
      SharedPtr<IShaderProgram> deferredLightingShaderProgram
    );
    void execute(IFrameBuffer* currentRenderTarget);
    void destroy();

  private:
    OpenGlGBuffer* m_gBuffer;
    OpenGlLightShadowManager& m_lightShadowManager;
    SharedPtr<IShaderProgram> m_deferredLightingShaderProgram;
  };
}
