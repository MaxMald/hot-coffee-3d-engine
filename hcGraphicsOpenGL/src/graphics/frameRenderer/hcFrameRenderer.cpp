#include "hc/graphics/frameRenderer/hcFrameRenderer.h"

#include "hc/graphics/hcDrawCommandUtilities.h"
#include "hc/graphics/frameRenderer/hcFrameRenderContext.h"
#include "hc/graphics/cubeMap/hcOpenGlCubeMap.h"

namespace hc
{
  static constexpr UInt32 CAMERA_FRAME_BINDING_POINT = 1;
  static constexpr UInt32 LIGHTS_BINDING_POINT = 2;
  static constexpr UInt32 LIGHT_SHADOWS_BINDING_POINT = 3;

  FrameRenderer::FrameRenderer() :
    m_forwardRenderPipeline(),
    m_deferredHybridRenderPipeline(),
    m_finalRenderPass(),
    m_frameBufferA(),
    m_lightFrameUBO(),
    m_cameraFrameUBO(),
    m_lightShadowManager(),
    m_drawCommands(),
    m_skybox(nullptr),
    m_currentRenderPipelineType(renderPipelineType::DeferredHybrid),
    m_currentRenderTarget(nullptr),
    m_initialized(false)
  {}

  FrameRenderer::~FrameRenderer()
  {
    destroy();
  }

  void FrameRenderer::initialize(
    const Rect<UInt32>& viewportRect,
    IShaderProgramManager & shaderProgramManager
  )
  {
    if (m_initialized)
      throw RuntimeErrorException("FrameRenderer is already initialized.");

    try
    {
      m_forwardRenderPipeline.initialize(shaderProgramManager);
      m_deferredHybridRenderPipeline.initialize(viewportRect, shaderProgramManager);
      m_finalRenderPass.initialize(shaderProgramManager.getBuiltInShaderProgram(builtInShaderProgramType::FinalPass));
      m_frameBufferA.initialize(viewportRect.width, viewportRect.height);
      m_lightFrameUBO.initialize(LightFrameData{});
      m_lightFrameUBO.bindBase(LIGHTS_BINDING_POINT);
      m_cameraFrameUBO.initialize(CameraFrameData{});
      m_cameraFrameUBO.bindBase(CAMERA_FRAME_BINDING_POINT);
      m_lightShadowManager.initialize(LIGHT_SHADOWS_BINDING_POINT);
    }
    catch (const Exception& e)
    {
      destroy();
      throw RuntimeErrorException("Failed to initialize FrameRenderer: " + String(e.what()));
    }

    m_initialized = true;
  }

  void FrameRenderer::setRenderPipeline(renderPipelineType::Type renderPipelineType)
  {
    m_currentRenderPipelineType = renderPipelineType;
  }

  renderPipelineType::Type FrameRenderer::getCurrentRenderPipelineType() const
  {
    return m_currentRenderPipelineType;
  }

  void FrameRenderer::uploadCameraFrameData(const CameraFrameData& cameraFrameData)
  {
    CameraFrameData transposedCameraData = cameraFrameData;
    transposedCameraData.projectionMatrix.transpose();
    transposedCameraData.viewMatrix.transpose();
    m_cameraFrameUBO.upload(transposedCameraData);
  }

  void FrameRenderer::uploadLightFrameData(const LightFrameData & lightFrameData)
  {
    m_lightFrameUBO.upload(lightFrameData);
  }

  void FrameRenderer::setSkybox(OpenGlCubeMap* skybox)
  {
    if (skybox && !skybox->isValid())
      throw InvalidArgumentException("Invalid skybox provided. The skybox must be initialized and valid.");
    m_skybox = skybox;
  }

  void FrameRenderer::removeSkybox()
  {
    m_skybox = nullptr;
  }

  void FrameRenderer::setRenderTarget(IFrameBuffer * frameBuffer)
  {
    if (frameBuffer)
    {
      if (!frameBuffer->isValid())
        throw InvalidArgumentException("Invalid framebuffer provided as render target.");
      frameBuffer->bind();
      m_currentRenderTarget = frameBuffer;
    }
    else
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      m_currentRenderTarget = nullptr;
    }
  }

  IFrameBuffer* FrameRenderer::getRenderTarget() const
  {
    return m_currentRenderTarget;
  }

  void FrameRenderer::removeRenderTarget()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_currentRenderTarget = nullptr;
  }

  void FrameRenderer::queueDrawCommand(const DrawCommand & drawCommand)
  {
    if (!drawCommand.material)
    {
      LogService::Error(
        "Draw command has no material assigned. Draw command not queued."
      );
      return ;
    }
    else if (!std::holds_alternative<OpenGlDrawData>(drawCommand.apiDrawData))
    {
      LogService::Error(
        "Draw command has invalid API draw data type, expected OpenGlDrawData. Draw command not queued."
      );
      return;
    }
    else
    {
      const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(drawCommand.apiDrawData);
      if (drawData.vao == 0)
      {
        LogService::Error("Draw command has invalid VAO (0). Draw command not queued.");
        return;
      }
    }

    m_drawCommands.push_back(drawCommand);
  }

  void FrameRenderer::execute()
  {
    assertIsInitialized();

    DrawCommandUtilities::SortDrawCommands(m_drawCommands);

    FrameRenderContext frameRenderContext;
    frameRenderContext.customFrameBuffer = &m_frameBufferA;
    frameRenderContext.skyboxCubeMap = m_skybox;

    if (m_currentRenderPipelineType == renderPipelineType::DeferredHybrid)
      m_deferredHybridRenderPipeline.execute(m_drawCommands, frameRenderContext);
    else if (m_currentRenderPipelineType == renderPipelineType::Forward)
      m_forwardRenderPipeline.execute(m_drawCommands, frameRenderContext);
    else
      throw RuntimeErrorException("Frame Renderer: Not implemented render pipeline type.");

    m_finalRenderPass.execute(m_frameBufferA.getColorTexture(), m_currentRenderTarget);
    copyDepthBuffer(m_frameBufferA, m_currentRenderTarget);
    m_drawCommands.clear();
  }

  OpenGlGBuffer& FrameRenderer::getGBuffer()
  {
    return m_deferredHybridRenderPipeline.getGBuffer();
  }

  void FrameRenderer::clearFrame()
  {
    assertIsInitialized();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (m_currentRenderPipelineType == renderPipelineType::DeferredHybrid)
      m_deferredHybridRenderPipeline.clearGBuffer();
    m_frameBufferA.clear(Color(0.0f, 0.0f, 0.0f, 1.0f));
  }

  void FrameRenderer::destroy()
  {
    m_forwardRenderPipeline.destroy();
    m_deferredHybridRenderPipeline.destroy();
    m_finalRenderPass.destroy();
    m_frameBufferA.destroy();
    m_lightShadowManager.destroy();
    m_lightFrameUBO.destroy();
    m_cameraFrameUBO.destroy();
    m_skybox = nullptr;
    m_initialized = false;
  }

  void FrameRenderer::onViewportChanged(const Rect<UInt32>&viewportRect)
  {
    m_deferredHybridRenderPipeline.updateViewportSize(viewportRect);
    m_frameBufferA.resize(viewportRect.width, viewportRect.height);
  }

  void FrameRenderer::assertIsInitialized() const
  {
    if (!m_initialized)
      throw RuntimeErrorException("FrameRenderer is not initialized.");
  }

  void FrameRenderer::copyDepthBuffer(IFrameBuffer& from, IFrameBuffer* to)
  {
    if (!from.isValid())
      throw RuntimeErrorException("Invalid framebuffer provided as source for depth buffer copy.");

    if (to)
    {
      if (!to->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");

      from.copyDepthTo(*to);
    }
    else
    {
      // If no destination render target, copy the depth buffer from the source
      // framebuffer to the default framebuffer

      from.bindForReadingOnly();
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
      glBlitFramebuffer(
        0, 0, from.getWidth(), from.getHeight(),
        0, 0, from.getWidth(), from.getHeight(),
        GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
      );
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
  }
}
