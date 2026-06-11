#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGBuffer;

  class DeferredGeometryRenderPass
  {
  public:
    DeferredGeometryRenderPass();
    ~DeferredGeometryRenderPass();

    void initialize(OpenGlGBuffer* gBuffer);
    void execute(const Vector<DrawCommand>& drawCommands);
    void destroy();

  private:
    OpenGlGBuffer* m_gBuffer;

    void executeTwoSidedDrawCommand(const DrawCommand& command);
  };
}
