#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class IRenderPipeline
  {
  public:
    virtual ~IRenderPipeline() = default;

    virtual void initialize() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void onFrameBegin() = 0;
    virtual void onFrameEnd() = 0;
    virtual void onViewportChanged(const Rect<UInt32>& viewportRect) = 0;
    virtual void executeDrawCommands(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    ) = 0;
    virtual void destroy() = 0;

  protected:
    IRenderPipeline() = default;
  };
}
