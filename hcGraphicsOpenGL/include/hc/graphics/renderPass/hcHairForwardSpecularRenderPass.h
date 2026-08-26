#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGBuffer;
  class OpenGlLightShadowManager;

  class HairForwardSpecularRenderPass
  {
  public:
    HairForwardSpecularRenderPass(OpenGlLightShadowManager& lightShadowManager);
    ~HairForwardSpecularRenderPass();

    void execute(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );

  private:
    OpenGlLightShadowManager& m_lightShadowManager;

    void executeTwoSidedDrawCommand(const DrawCommand& command);
  };
}
