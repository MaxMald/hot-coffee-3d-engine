#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGBuffer;

  class ForwardOpaqueRenderPass
  {
  public:
    ForwardOpaqueRenderPass();
    ~ForwardOpaqueRenderPass();

    void execute(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );

  private:
    void executeTwoSidedDrawCommand(const DrawCommand& command);
  };
}
