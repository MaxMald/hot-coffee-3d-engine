#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGBuffer;
  class OpenGlLightShadowMapManager;
  class IDataBlockManager;

  class HairForwardSpecularRenderPass
  {
  public:
    HairForwardSpecularRenderPass(
      OpenGlLightShadowMapManager& lightShadowMapManager,
      IDataBlockManager& dataBlockManager
    );
    ~HairForwardSpecularRenderPass();

    void execute(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );

  private:
    OpenGlLightShadowMapManager& m_lightShadowMapManager;
    IDataBlockManager& m_dataBlockManager;

    void executeTwoSidedDrawCommand(const DrawCommand& command);
  };
}
