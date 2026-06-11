#include "hc/graphics/renderPass/hcDeferredLightingRenderPass.h"

#include <GL/glew.h>
#include "hc/graphics/resource/frameBuffer/hcOpenGlGBuffer.h"

namespace hc
{
  DeferredLightingRenderPass::DeferredLightingRenderPass() :
    m_gBuffer(nullptr),
    m_deferredLightingShaderProgram(nullptr)
  {}

  DeferredLightingRenderPass::~DeferredLightingRenderPass()
  {}

  void DeferredLightingRenderPass::initialize(
    OpenGlGBuffer* gBuffer,
    SharedPtr<IShaderProgram> deferredLightingShaderProgram
  )
  {
    if (!gBuffer || !gBuffer->isValid())
      throw InvalidArgumentException("Invalid G-buffer provided for deferred lighting pass.");

    if (!deferredLightingShaderProgram || !deferredLightingShaderProgram->isValid())
      throw InvalidArgumentException("Invalid shader program provided for deferred lighting pass.");

    m_gBuffer = gBuffer;
    m_deferredLightingShaderProgram = deferredLightingShaderProgram;
  }

  void DeferredLightingRenderPass::execute(IFrameBuffer* currentRenderTarget)
  {
    if (!m_gBuffer || !m_gBuffer->isValid())
      throw RuntimeErrorException("DeferredLightingRenderPass: G-buffer is not initialized or valid.");

    if (!m_deferredLightingShaderProgram || !m_deferredLightingShaderProgram->isValid())
      throw RuntimeErrorException("DeferredLightingRenderPass: Deferred lighting shader program is not initialized or valid.");

    if (currentRenderTarget)
    {
      if (!currentRenderTarget->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");

      currentRenderTarget->bind();
    }

    m_deferredLightingShaderProgram->bind();
    m_gBuffer->bindGTexturesForReading();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_gBuffer->unbind();

    if (currentRenderTarget)
      currentRenderTarget->unbind();
  }

  void DeferredLightingRenderPass::destroy()
  {
    m_gBuffer = nullptr;
    m_deferredLightingShaderProgram.reset();
  }
}
