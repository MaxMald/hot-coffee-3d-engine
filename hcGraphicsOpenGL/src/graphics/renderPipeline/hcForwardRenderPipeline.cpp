#include "hc/graphics/renderPipeline/hcForwardRenderPipeline.h"
#include <GL/glew.h>

namespace hc
{
  void ForwardRenderPipeline::executeDrawCommands(
    const Vector<DrawCommand>&drawCommands,
    IFrameBuffer * currentRenderTarget
  )
  {
    if (currentRenderTarget)
    {
      if (!currentRenderTarget->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");
      currentRenderTarget->bind();
    }

    for (const DrawCommand& command : drawCommands)
      executeDrawCommand(command);

    if (currentRenderTarget)
      currentRenderTarget->unbind();
  }

  void ForwardRenderPipeline::executeDrawCommand(const DrawCommand & command)
  {
    if (command.material->getRenderMode() == materialRenderMode::Type::Transparent)
      executeTransparentDrawCommand(command);
    else
      executeOpaqueDrawCommand(command);
  }

  void ForwardRenderPipeline::executeTransparentDrawCommand(const DrawCommand& command)
  {
    // Save current state before rendering transparent object

    GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);

    GLint blendSrcRGB, blendDstRGB, blendSrcAlpha, blendDstAlpha;
    glGetIntegerv(GL_BLEND_SRC_RGB, &blendSrcRGB);
    glGetIntegerv(GL_BLEND_DST_RGB, &blendDstRGB);
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrcAlpha);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDstAlpha);

    // Set state for rendering transparent object

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    if (command.material->isDoubleSided())
    {
      executeTransparentTwoSidedDrawCommand(command);
    }
    else // Single-sided transparent object
    {
      const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

      glBindVertexArray(drawData.vao);
      bindMaterialForDrawCommand(command);
      drawElements(command, drawData.drawMode);
      unbindMaterialForDrawCommand(command);
      glBindVertexArray(0);
    }

    // Restore previous state after rendering transparent object

    if (!blendEnabled)
      glDisable(GL_BLEND);

    glBlendFuncSeparate(
      static_cast<GLenum>(blendSrcRGB),
      static_cast<GLenum>(blendDstRGB),
      static_cast<GLenum>(blendSrcAlpha),
      static_cast<GLenum>(blendDstAlpha)
    );

    glDepthMask(depthMaskEnabled);
  }

  void ForwardRenderPipeline::executeTransparentTwoSidedDrawCommand(
    const DrawCommand& command
  )
  {
    // Save current state before rendering transparent object

    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    GLint cullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);

    // Set state for rendering transparent object

    glEnable(GL_CULL_FACE);

    const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

    glBindVertexArray(drawData.vao);
    bindMaterialForDrawCommand(command);

    // Pass 1: Render back faces first
    glCullFace(GL_FRONT);
    drawElements(command, drawData.drawMode);

    // Pass 2: Render front faces
    glCullFace(GL_BACK);
    drawElements(command, drawData.drawMode);

    unbindMaterialForDrawCommand(command);
    glBindVertexArray(0);

    // Restore previous state after rendering transparent two-sided object

    if (!cullFaceEnabled)
      glDisable(GL_CULL_FACE);

    glCullFace(static_cast<GLenum>(cullFaceMode));
  }

  void ForwardRenderPipeline::executeOpaqueDrawCommand(const DrawCommand& command)
  {
    if (command.material->isDoubleSided())
    {
      executeOpaqueTwoSidedDrawCommand(command);
    }
    else // Single-sided opaque object
    {
      const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

      glBindVertexArray(drawData.vao);
      bindMaterialForDrawCommand(command);
      drawElements(command, drawData.drawMode);
      unbindMaterialForDrawCommand(command);
      glBindVertexArray(0);
    }
  }

  void ForwardRenderPipeline::executeOpaqueTwoSidedDrawCommand(
    const DrawCommand & command
  )
  {
    // Save current state before rendering opaque object

    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    GLint cullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);

    // Set state for rendering opaque object

    glDisable(GL_CULL_FACE);

    const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

    glBindVertexArray(drawData.vao);
    bindMaterialForDrawCommand(command);
    drawElements(command, drawData.drawMode);
    unbindMaterialForDrawCommand(command);
    glBindVertexArray(0);

    // Restore previous state after rendering opaque two-sided object

    if (cullFaceEnabled)
      glEnable(GL_CULL_FACE);

    glCullFace(static_cast<GLenum>(cullFaceMode));
  }

  void ForwardRenderPipeline::bindMaterialForDrawCommand(const DrawCommand & command)
  {
    command.material->bind(renderPassType::Type::Forward);
    command.material->updateModelMatrix(command.modelMatrix, renderPassType::Type::Forward);
  }

  void ForwardRenderPipeline::unbindMaterialForDrawCommand(const DrawCommand & command)
  {
    command.material->unbind();
  }

  void ForwardRenderPipeline::drawElements(const DrawCommand & command, UInt32 drawMode)
  {
    glDrawElements(
      static_cast<GLenum>(drawMode),
      static_cast<GLsizei>(command.indexCount),
      GL_UNSIGNED_INT,
      reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
    );
  }
}
