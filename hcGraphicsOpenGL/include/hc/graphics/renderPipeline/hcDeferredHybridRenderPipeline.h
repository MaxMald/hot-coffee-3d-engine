#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/frameBuffer/hcOpenGlGBuffer.h"

namespace hc
{
  class ForwardRenderPipeline;

  class DeferredHybridRenderPipeline
  {
  public:
    DeferredHybridRenderPipeline(ForwardRenderPipeline& forwardRenderPipeline);
    ~DeferredHybridRenderPipeline() = default;

    void initialize(
      const Rect<UInt32>& viewportRect,
      SharedPtr<IShaderProgram> deferredLightingShaderProgram
    );
    void clearGBuffer();
    void updateViewportSize(const Rect<UInt32>& viewportRect);
    void executeDrawCommands(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );
    void destroy();

    OpenGlGBuffer& getGBuffer();

  private:
    ForwardRenderPipeline& m_forwardRenderPipeline;
    OpenGlGBuffer m_gBuffer;
    SharedPtr<IShaderProgram> m_deferredLightingShaderProgram;
    Vector<DrawCommand> m_deferredGeometryPassCommands;
    Vector<DrawCommand> m_deferredForwardPassCommands;

    void executeDeferredGeometryPass(const Vector<DrawCommand>& drawCommands);
    void executeTwoSidedDrawCommand(const DrawCommand& command);
    void executeDeferredLightingPass();
    void bindMaterialForDrawCommand(const DrawCommand& command);
    void unbindMaterialForDrawCommand(const DrawCommand& command);
    void drawElements(const DrawCommand& command, UInt32 drawMode);
  };
}
