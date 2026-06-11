#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlCubeMap;

  struct FrameRenderContext
  {
    IFrameBuffer* customFrameBuffer = nullptr;
    OpenGlCubeMap* skyboxCubeMap = nullptr;
  };
}
