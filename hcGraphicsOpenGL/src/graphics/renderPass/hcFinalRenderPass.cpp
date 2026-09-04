#include "hc/graphics/renderPass/hcFinalRenderPass.h"

#include <GL/glew.h>

namespace hc
{
  FinalRenderPass::FinalRenderPass()
  {}

  FinalRenderPass::~FinalRenderPass()
  {
    destroy();
  }

  void FinalRenderPass::initialize(SharedPtr<IShaderProgram> finalPassShaderProgram)
  {
    if (!finalPassShaderProgram || !finalPassShaderProgram->isValid())
      throw InvalidArgumentException("Invalid shader program provided to FinalRenderPass.");
    m_finalPassShaderProgram = finalPassShaderProgram;
  }

  void FinalRenderPass::execute(
    ITexture& sceneTexture,
    IFrameBuffer* finalFrameBuffer
  )
  {
    if (!m_finalPassShaderProgram || !m_finalPassShaderProgram->isValid())
      throw RuntimeErrorException("FinalRenderPass: Shader program is not initialized or valid.");

    if (!sceneTexture.isValid())
      throw RuntimeErrorException("FinalRenderPass: Scene texture is not initialized or valid.");

    if (finalFrameBuffer)
    {
      if (!finalFrameBuffer->isValid())
        throw RuntimeErrorException("FinalRenderPass: Final framebuffer is not initialized or valid.");
      finalFrameBuffer->bind();
    }
    else
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    m_finalPassShaderProgram->bind();
    sceneTexture.bind(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    sceneTexture.unbind(0);

    if (finalFrameBuffer)
      finalFrameBuffer->unbind();
  }

  void FinalRenderPass::destroy()
  {
    m_finalPassShaderProgram.reset();
  }
}
