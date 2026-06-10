#include "hc/graphics/renderPass/hcDeferredGeometryRenderPass.h"

#include <GL/glew.h>
#include "hc/graphics/resource/frameBuffer/hcOpenGlGBuffer.h"

namespace hc
{
  DeferredGeometryRenderPass::DeferredGeometryRenderPass()
    : m_gBuffer(nullptr)
  {}

  DeferredGeometryRenderPass::~DeferredGeometryRenderPass()
  {}

  void DeferredGeometryRenderPass::initialize(OpenGlGBuffer* gBuffer)
  {
    if (!gBuffer || !gBuffer->isValid())
      throw InvalidArgumentException("Invalid G-buffer provided for deferred geometry pass.");
    m_gBuffer = gBuffer;
  }

  void DeferredGeometryRenderPass::execute(const Vector<DrawCommand>& drawCommands)
  {
    if (!m_gBuffer || !m_gBuffer->isValid())
      throw RuntimeErrorException("DeferredOpaqueRenderPass: G-buffer is not initialized or valid.");

    m_gBuffer->bind();

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
        command.material->bind(renderPassType::Type::DeferredGeometry);
        command.material->updateModelMatrix(
          command.modelMatrix,
          renderPassType::Type::DeferredGeometry
        );

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

    m_gBuffer->unbind();
  }

  void DeferredGeometryRenderPass::destroy()
  {
    m_gBuffer = nullptr;
  }

  void DeferredGeometryRenderPass::executeTwoSidedDrawCommand(
    const DrawCommand& command
  )
  {
    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);

    glDisable(GL_CULL_FACE);

    const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

    glBindVertexArray(drawData.vao);
    command.material->bind(renderPassType::Type::DeferredGeometry);
    command.material->updateModelMatrix(
      command.modelMatrix,
      renderPassType::Type::DeferredGeometry
    );

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
  }
}
