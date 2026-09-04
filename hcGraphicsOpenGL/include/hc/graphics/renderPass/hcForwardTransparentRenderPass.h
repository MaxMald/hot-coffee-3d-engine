#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class IDataBlockManager;

  class ForwardTransparentRenderPass
  {
  public:
    ForwardTransparentRenderPass(IDataBlockManager& dataBlockManager);
    ~ForwardTransparentRenderPass();

    void execute(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );

  private:
    IDataBlockManager& m_dataBlockManager;

    void executeTwoSidedDrawCommand(const DrawCommand& command);
  };
}
