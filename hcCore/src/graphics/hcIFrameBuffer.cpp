#include "hc/graphics/hcIFrameBuffer.h"

namespace hc
{
  void FrameBufferDeleter::operator()(IFrameBuffer* frameBuffer) const
  {
    if (frameBuffer)
    {
      frameBuffer->destroy();
      delete frameBuffer;
    }
  }
}
