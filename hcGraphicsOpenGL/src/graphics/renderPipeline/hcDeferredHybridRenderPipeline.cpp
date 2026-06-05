#include "hc/graphics/renderPipeline/hcDeferredHybridRenderPipeline.h"
#include "hc/graphics/renderPipeline/hcForwardRenderPipeline.h"
#include "hc/graphics/hcDrawCommandUtilities.h"

namespace hc
{
  DeferredHybridRenderPipeline::DeferredHybridRenderPipeline(
    ForwardRenderPipeline& forwardRenderPipeline
  ) :
    m_forwardRenderPipeline(forwardRenderPipeline),
    m_gBuffer(),
    m_deferredLightingShaderProgram(nullptr),
    m_deferredGeometryPassCommands(),
    m_deferredForwardPassCommands()
  {}

  void DeferredHybridRenderPipeline::initialize(
    const Rect<UInt32>& viewportRect,
    SharedPtr<IShaderProgram> deferredLightingShaderProgram
  )
  {
    if (!deferredLightingShaderProgram || !deferredLightingShaderProgram->isValid())
      throw InvalidArgumentException("Invalid shader program provided for deferred lighting pass.");

    m_gBuffer.initialize(viewportRect.width, viewportRect.height);
    m_deferredLightingShaderProgram = deferredLightingShaderProgram;
  }

  void DeferredHybridRenderPipeline::clearGBuffer()
  {
    m_gBuffer.clear(Color::Black());
  }

  void DeferredHybridRenderPipeline::updateViewportSize(const Rect<UInt32>& viewportRect)
  {
    m_gBuffer.resize(viewportRect.width, viewportRect.height);
  }

  void DeferredHybridRenderPipeline::executeDrawCommands(
    const Vector<DrawCommand>& drawCommands,
    IFrameBuffer* currentRenderTarget
  )
  {
    if (currentRenderTarget)
    {
      if (!currentRenderTarget->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");
    }

    m_deferredGeometryPassCommands.clear();
    m_deferredForwardPassCommands.clear();
    DrawCommandUtilities::SplitDrawCommandsByPipelinePath(
      drawCommands,
      m_deferredGeometryPassCommands,
      m_deferredForwardPassCommands
    );

    executeDeferredGeometryPass(m_deferredGeometryPassCommands);

    if (currentRenderTarget)
      currentRenderTarget->bind();

    executeDeferredLightingPass();

    if (currentRenderTarget)
      m_gBuffer.copyDepthTo(*currentRenderTarget);
    else
    {
      // If no custom render target, copy the depth buffer from the GBuffer to the default
      // framebuffer

      m_gBuffer.bindForReadingOnly();
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
      glBlitFramebuffer(
        0, 0, m_gBuffer.getWidth(), m_gBuffer.getHeight(),
        0, 0, m_gBuffer.getWidth(), m_gBuffer.getHeight(),
        GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
      );
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    m_forwardRenderPipeline.executeDrawCommands(
      m_deferredForwardPassCommands,
      currentRenderTarget
    );

    if (currentRenderTarget)
      currentRenderTarget->unbind();
  }

  void DeferredHybridRenderPipeline::destroy()
  {
    m_deferredForwardPassCommands.clear();
    m_deferredGeometryPassCommands.clear();
    m_gBuffer.destroy();
  }

  OpenGlGBuffer& DeferredHybridRenderPipeline::getGBuffer()
  {
    return m_gBuffer;
  }

  void DeferredHybridRenderPipeline::executeDeferredGeometryPass(
    const Vector<DrawCommand>& drawCommands
  )
  {
    m_gBuffer.bind();

    for (const DrawCommand& command : drawCommands)
    {
      if (command.material->isDoubleSided())
      {
        executeTwoSidedDrawCommand(command);
      }
      else
      {
        const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

        glBindVertexArray(drawData.vao);
        bindMaterialForDrawCommand(command);
        drawElements(command, drawData.drawMode);
        unbindMaterialForDrawCommand(command);
        glBindVertexArray(0);
      }
    }

    m_gBuffer.unbind();
  }

  void DeferredHybridRenderPipeline::executeTwoSidedDrawCommand(
    const DrawCommand& command
  )
  {
    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);

    glDisable(GL_CULL_FACE);

    const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

    glBindVertexArray(drawData.vao);
    bindMaterialForDrawCommand(command);
    drawElements(command, drawData.drawMode);
    unbindMaterialForDrawCommand(command);
    glBindVertexArray(0);

    if (cullFaceEnabled)
      glEnable(GL_CULL_FACE);
  }

  void DeferredHybridRenderPipeline::executeDeferredLightingPass()
  {
    m_deferredLightingShaderProgram->bind();
    m_gBuffer.bindGTexturesForReading();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_gBuffer.unbind();
  }

  void DeferredHybridRenderPipeline::bindMaterialForDrawCommand(
    const DrawCommand& command
  )
  {
    command.material->bind(renderPassType::Type::DeferredGeometry);
    command.material->updateModelMatrix(
      command.modelMatrix,
      renderPassType::Type::DeferredGeometry
    );
  }

  void DeferredHybridRenderPipeline::unbindMaterialForDrawCommand(
    const DrawCommand& command
  )
  {
    command.material->unbind();
  }

  void DeferredHybridRenderPipeline::drawElements(
    const DrawCommand& command,
    UInt32 drawMode
  )
  {
    glDrawElements(
      drawMode,
      static_cast<GLsizei>(command.indexCount),
      GL_UNSIGNED_INT,
      reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
    );
  }
}
