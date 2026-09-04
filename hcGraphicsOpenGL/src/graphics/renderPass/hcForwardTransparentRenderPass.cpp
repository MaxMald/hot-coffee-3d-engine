#include "hc/graphics/renderPass/hcForwardTransparentRenderPass.h"

#include <GL/glew.h>
#include <hc/graphics/resource/dataBlock/hcIDataBlockManager.h>

namespace hc
{
  ForwardTransparentRenderPass::ForwardTransparentRenderPass(
    IDataBlockManager& dataBlockManager
  )
    : m_dataBlockManager(dataBlockManager)
  {}

  ForwardTransparentRenderPass::~ForwardTransparentRenderPass()
  {}

  void ForwardTransparentRenderPass::execute(
    const Vector<DrawCommand>& drawCommands,
    IFrameBuffer* currentRenderTarget
  )
  {
    if (currentRenderTarget)
    {
      if (!currentRenderTarget->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");
      currentRenderTarget->bind();
    }

    GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    if (!depthTestEnabled)
      glEnable(GL_DEPTH_TEST);

    GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);

    GLint blendSrcRGB, blendDstRGB, blendSrcAlpha, blendDstAlpha;
    glGetIntegerv(GL_BLEND_SRC_RGB, &blendSrcRGB);
    glGetIntegerv(GL_BLEND_DST_RGB, &blendDstRGB);
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrcAlpha);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDstAlpha);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    for (const DrawCommand& command : drawCommands)
    {
      if (command.material->isDoubleSided())
      {
        executeTwoSidedDrawCommand(command);
      }
      else // Single-sided transparent object
      {
        const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

        glBindVertexArray(drawData.vao);
        command.material->bind(
          renderPassType::Type::ForwardTransparent,
          m_dataBlockManager
        );

        // Update the model matrix uniform in the material
        dataBlockStructure::ObjectData objectData;
        objectData.modelMatrix = command.modelMatrix.transposed();
        m_dataBlockManager.upload(dataBlockType::Object, &objectData);
        m_dataBlockManager.bind(dataBlockType::Object);

        glDrawElements(
          static_cast<GLenum>(drawData.drawMode),
          static_cast<GLsizei>(command.indexCount),
          GL_UNSIGNED_INT,
          reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
        );

        command.material->unbind();
        glBindVertexArray(0);
      }
    }

    if (!blendEnabled)
      glDisable(GL_BLEND);

    glBlendFuncSeparate(
      static_cast<GLenum>(blendSrcRGB),
      static_cast<GLenum>(blendDstRGB),
      static_cast<GLenum>(blendSrcAlpha),
      static_cast<GLenum>(blendDstAlpha)
    );

    glDepthMask(depthMaskEnabled);

    if (!depthTestEnabled)
      glDisable(GL_DEPTH_TEST);

    if (currentRenderTarget)
      currentRenderTarget->unbind();
  }

  void ForwardTransparentRenderPass::executeTwoSidedDrawCommand(
    const DrawCommand& command
  )
  {
    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    GLint cullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);

    glEnable(GL_CULL_FACE);

    const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

    glBindVertexArray(drawData.vao);
    command.material->bind(
      renderPassType::Type::ForwardTransparent,
      m_dataBlockManager
    );

    // Update the model matrix uniform in the material
    dataBlockStructure::ObjectData objectData;
    objectData.modelMatrix = command.modelMatrix.transposed();
    m_dataBlockManager.upload(dataBlockType::Object, &objectData);
    m_dataBlockManager.bind(dataBlockType::Object);

    // Pass 1: Render back faces first
    glCullFace(GL_FRONT);
    glDrawElements(
      static_cast<GLenum>(drawData.drawMode),
      static_cast<GLsizei>(command.indexCount),
      GL_UNSIGNED_INT,
      reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
    );

    // Pass 2: Render front faces
    glCullFace(GL_BACK);
    glDrawElements(
      static_cast<GLenum>(drawData.drawMode),
      static_cast<GLsizei>(command.indexCount),
      GL_UNSIGNED_INT,
      reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
    );

    command.material->unbind();
    glBindVertexArray(0);

    if (!cullFaceEnabled)
      glDisable(GL_CULL_FACE);

    glCullFace(static_cast<GLenum>(cullFaceMode));
  }
}
