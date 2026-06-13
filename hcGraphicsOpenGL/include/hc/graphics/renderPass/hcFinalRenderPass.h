#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class FinalRenderPass
  {
  public:
    FinalRenderPass();
    ~FinalRenderPass();

    void initialize(SharedPtr<IShaderProgram> finalPassShaderProgram);
    void execute(ITexture& sceneTexture, IFrameBuffer* finalFrameBuffer);
    void destroy();

  private:
    SharedPtr<IShaderProgram> m_finalPassShaderProgram;
  };
}
