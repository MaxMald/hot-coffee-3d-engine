#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class ForwardRenderPipeline
  {
  public:
    ForwardRenderPipeline() = default;
    ~ForwardRenderPipeline() = default;

    void executeDrawCommands(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );

  private:
    void executeDrawCommand(const DrawCommand& command);
    void executeTransparentDrawCommand(const DrawCommand& command);
    void executeTransparentTwoSidedDrawCommand(const DrawCommand& command);
    void executeOpaqueDrawCommand(const DrawCommand& command);
    void executeOpaqueTwoSidedDrawCommand(const DrawCommand& command);
    void bindMaterialForDrawCommand(const DrawCommand& command);
    void unbindMaterialForDrawCommand(const DrawCommand& command);
    void drawElements(const DrawCommand& command, UInt32 drawMode);
  };
}
