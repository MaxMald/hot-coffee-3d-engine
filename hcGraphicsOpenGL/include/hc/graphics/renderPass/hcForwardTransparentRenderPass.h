#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class ForwardTransparentRenderPass
  {
  public:
    ForwardTransparentRenderPass();
    ~ForwardTransparentRenderPass();

    void execute(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );

  private:
    void executeTwoSidedDrawCommand(const DrawCommand& command);
  };
}
