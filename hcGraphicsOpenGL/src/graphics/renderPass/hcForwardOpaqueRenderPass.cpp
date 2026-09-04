#include "hc/graphics/renderPass/hcForwardOpaqueRenderPass.h"

#include <GL/glew.h>
#include <hc/graphics/resource/dataBlock/hcIDataBlockManager.h>

namespace hc
{
  ForwardOpaqueRenderPass::ForwardOpaqueRenderPass(IDataBlockManager& dataBlockManager)
    : m_dataBlockManager(dataBlockManager)
  {}

  ForwardOpaqueRenderPass::~ForwardOpaqueRenderPass()
  {}

  void ForwardOpaqueRenderPass::execute(
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

    for (const DrawCommand& command : drawCommands)
    {
      if (command.material->isDoubleSided())
      {
        executeTwoSidedDrawCommand(command);
      }
      else // One-sided opaque object
      {
        const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

        glBindVertexArray(drawData.vao);
        command.material->bind(
          renderPassType::Type::Forward,
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

    if (currentRenderTarget)
      currentRenderTarget->unbind();
  }

  void ForwardOpaqueRenderPass::executeTwoSidedDrawCommand(
    const DrawCommand& command
  )
  {
    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    GLint cullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);

    glDisable(GL_CULL_FACE);

    const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

    glBindVertexArray(drawData.vao);
    command.material->bind(
      renderPassType::Type::Forward,
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

    if (cullFaceEnabled)
      glEnable(GL_CULL_FACE);

    glCullFace(static_cast<GLenum>(cullFaceMode));
  }
}
