#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGBuffer;

  class DeferredLightingRenderPass
  {
  public:
    DeferredLightingRenderPass();
    ~DeferredLightingRenderPass();

    void initialize(
      OpenGlGBuffer* gBuffer,
      SharedPtr<IShaderProgram> deferredLightingShaderProgram
    );
    void execute(IFrameBuffer* currentRenderTarget);
    void destroy();

  private:
    OpenGlGBuffer* m_gBuffer;
    SharedPtr<IShaderProgram> m_deferredLightingShaderProgram;
  };
}
