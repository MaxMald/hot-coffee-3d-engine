#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc::graphics::shadows
{
  class OpenGlLightShadowMapManager;
}

namespace hc
{
  class OpenGlGBuffer;
  class IDataBlockManager;

  class HairForwardSpecularRenderPass
  {
  public:
    HairForwardSpecularRenderPass(
      graphics::shadows::OpenGlLightShadowMapManager& lightShadowMapManager,
      IDataBlockManager& dataBlockManager
    );
    ~HairForwardSpecularRenderPass();

    void execute(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );

  private:
    graphics::shadows::OpenGlLightShadowMapManager& m_lightShadowMapManager;
    IDataBlockManager& m_dataBlockManager;

    void executeTwoSidedDrawCommand(const DrawCommand& command);
  };
}
