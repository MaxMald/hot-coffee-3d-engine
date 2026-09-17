#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGBuffer;
  class IDataBlockManager;

  class ForwardOpaqueRenderPass
  {
  public:
    ForwardOpaqueRenderPass(IDataBlockManager& dataBlockManager);
    ~ForwardOpaqueRenderPass();

    void execute(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );

  private:
    IDataBlockManager& m_dataBlockManager;

    void executeTwoSidedDrawCommand(const DrawCommand& command);
  };
}
