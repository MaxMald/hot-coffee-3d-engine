#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGBuffer;
  class IDataBlockManager;

  class DeferredGeometryRenderPass
  {
  public:
    DeferredGeometryRenderPass(IDataBlockManager& dataBlockManager);
    ~DeferredGeometryRenderPass();

    void initialize(OpenGlGBuffer* gBuffer);
    void execute(const Vector<DrawCommand>& drawCommands);
    void destroy();

  private:
    OpenGlGBuffer* m_gBuffer;
    IDataBlockManager& m_dataBlockManager;

    void executeTwoSidedDrawCommand(const DrawCommand& command);
  };
}
