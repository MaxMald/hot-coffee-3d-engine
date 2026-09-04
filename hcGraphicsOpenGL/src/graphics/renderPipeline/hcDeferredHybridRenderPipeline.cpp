#include "hc/graphics/renderPipeline/hcDeferredHybridRenderPipeline.h"

#include <GL/glew.h>
#include <hc/graphics/resource/dataBlock/hcIDataBlockManager.h>

#include "hc/graphics/hcDrawCommandUtilities.h"
#include "hc/graphics/frameRenderer/hcFrameRenderContext.h"
#include "hc/graphics/lightShadowManager/hcOpenGlLightShadowMapManager.h"

namespace hc
{
  DeferredHybridRenderPipeline::DeferredHybridRenderPipeline(
    IDataBlockManager& dataBlockManager,
    OpenGlLightShadowMapManager& lightShadowMapManager
  ) :
    m_deferredGeometryRenderPass(dataBlockManager),
    m_deferredLightingRenderPass(lightShadowMapManager),
    m_forwardOpaqueRenderPass(dataBlockManager),
    m_forwardTransparentRenderPass(dataBlockManager),
    m_hairForwardSpecularRenderPass(lightShadowMapManager, dataBlockManager),
    m_skyboxRenderPass(),
    m_gBuffer(),
    m_deferredOpaqueCommands(),
    m_forwardOpaqueCommands(),
    m_forwardTransparentCommands(),
    m_hairForwardSpecularCommands(),
    m_lightShadowMapManager(lightShadowMapManager),
    m_initialized(false)
  {}

  DeferredHybridRenderPipeline::~DeferredHybridRenderPipeline()
  {
    destroy();
  }

  void DeferredHybridRenderPipeline::destroy()
  {
    m_deferredOpaqueCommands.clear();
    m_forwardOpaqueCommands.clear();
    m_forwardTransparentCommands.clear();
    m_hairForwardSpecularCommands.clear();
    m_deferredGeometryRenderPass.destroy();
    m_deferredLightingRenderPass.destroy();
    m_skyboxRenderPass.destroy();
    m_gBuffer.destroy();
    m_initialized = false;
  }

  void DeferredHybridRenderPipeline::initialize(
    const Rect<UInt32>& viewportRect,
    IShaderProgramManager& shaderProgramManager
  )
  {
    if (m_initialized)
      throw RuntimeErrorException("DeferredHybridRenderPipeline is already initialized.");

    try
    {
      m_gBuffer.initialize(viewportRect.width, viewportRect.height);
      m_deferredGeometryRenderPass.initialize(&m_gBuffer);
      m_deferredLightingRenderPass.initialize(
        &m_gBuffer,
        shaderProgramManager.getBuiltInShaderProgram(builtInShaderProgramType::DeferredLighting)
      );
      m_skyboxRenderPass.initialize(
        shaderProgramManager.getBuiltInShaderProgram(builtInShaderProgramType::Skybox)
      );
    }
    catch (const Exception& e)
    {
      destroy();
      throw RuntimeErrorException(
        "Failed to initialize DeferredHybridRenderPipeline: " + String(e.what())
      );
    }
    
    m_initialized = true;
  }

  void DeferredHybridRenderPipeline::clearGBuffer()
  {
    m_gBuffer.clear(Color::Black());
  }

  void DeferredHybridRenderPipeline::updateViewportSize(const Rect<UInt32>& viewportRect)
  {
    m_gBuffer.resize(viewportRect.width, viewportRect.height);
  }

  void DeferredHybridRenderPipeline::execute(
    const Vector<DrawCommand>& drawCommands,
    const FrameRenderContext& frameRenderContext
  )
  {
    assertIsInitialized();

    m_deferredOpaqueCommands.clear();
    m_forwardOpaqueCommands.clear();
    m_forwardTransparentCommands.clear();
    m_hairForwardSpecularCommands.clear();

    SplitDrawCommandsByRenderPass(
      drawCommands,
      m_deferredOpaqueCommands,
      m_forwardOpaqueCommands,
      m_forwardTransparentCommands,
      m_hairForwardSpecularCommands
    );

    m_deferredGeometryRenderPass.execute(m_deferredOpaqueCommands);
    m_deferredLightingRenderPass.execute(frameRenderContext.customFrameBuffer);
    copyDepthBufferToCurrentRenderTarget(frameRenderContext.customFrameBuffer);
    m_forwardOpaqueRenderPass.execute(m_forwardOpaqueCommands, frameRenderContext.customFrameBuffer);
    m_hairForwardSpecularRenderPass.execute(m_hairForwardSpecularCommands, frameRenderContext.customFrameBuffer);

    if (frameRenderContext.skyboxCubeMap)
      m_skyboxRenderPass.execute(frameRenderContext.skyboxCubeMap, frameRenderContext.customFrameBuffer);

    m_forwardTransparentRenderPass.execute(m_forwardTransparentCommands, frameRenderContext.customFrameBuffer);
  }

  OpenGlGBuffer& DeferredHybridRenderPipeline::getGBuffer()
  {
    return m_gBuffer;
  }

  void DeferredHybridRenderPipeline::SplitDrawCommandsByRenderPass(
    const Vector<DrawCommand>& drawCommands,
    Vector<DrawCommand>& deferredOpaqueCommands,
    Vector<DrawCommand>& forwardOpaqueCommands,
    Vector<DrawCommand>& forwardTransparentCommands,
    Vector<DrawCommand>& hairForwardSpecularCommands
  )
  {
    for (SizeT i = 0; i < drawCommands.size(); ++i)
    {
      const DrawCommand& cmd = drawCommands[i];

      if (!cmd.material)
        continue;

      if (cmd.material->getMaterialType() == materialType::Hair)
      {
        deferredOpaqueCommands.push_back(cmd);
        hairForwardSpecularCommands.push_back(cmd);
        forwardTransparentCommands.push_back(cmd);
        continue;
      }

      if (cmd.material->getRenderMode() == materialRenderMode::Type::Transparent)
      {
        forwardTransparentCommands.push_back(cmd);
        continue;
      }

      if (cmd.material->getMaterialType() == materialType::Unlit)
      {
        forwardOpaqueCommands.push_back(cmd);
        continue;
      }

      deferredOpaqueCommands.push_back(cmd);
    }
  }

  void DeferredHybridRenderPipeline::copyDepthBufferToCurrentRenderTarget(
    IFrameBuffer* currentRenderTarget
  )
  {
    if (currentRenderTarget)
    {
      if (!currentRenderTarget->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");

      m_gBuffer.copyDepthTo(*currentRenderTarget);
    }
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
  }

  void DeferredHybridRenderPipeline::assertIsInitialized() const
  {
    if (!m_initialized)
      throw RuntimeErrorException("DeferredHybridRenderPipeline is not initialized.");
  }
}
