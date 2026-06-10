#include "hc/graphics/frameRenderer/hcFrameRenderer.h"

#include "hc/graphics/hcDrawCommandUtilities.h"

namespace hc
{
  static constexpr UInt32 CAMERA_FRAME_BINDING_POINT = 1;
  static constexpr UInt32 LIGHTS_BINDING_POINT = 2;

  FrameRenderer::FrameRenderer() :
    m_forwardRenderPipeline(),
    m_deferredHybridRenderPipeline(),
    m_lightFrameUBO(),
    m_cameraFrameUBO(),
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
      m_lightFrameUBO.initialize(LightFrameData{});
      m_lightFrameUBO.bindBase(LIGHTS_BINDING_POINT);
      m_cameraFrameUBO.initialize(CameraFrameData{});
      m_cameraFrameUBO.bindBase(CAMERA_FRAME_BINDING_POINT);
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

  void FrameRenderer::setSkybox(const SharedPtr<ICubeMap>&skybox)
  {
    m_skybox = skybox;
  }

  void FrameRenderer::removeSkybox()
  {
    m_skybox.reset();
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

    if (m_currentRenderPipelineType == renderPipelineType::DeferredHybrid)
      m_deferredHybridRenderPipeline.execute(m_drawCommands, m_currentRenderTarget);
    else if (m_currentRenderPipelineType == renderPipelineType::Forward)
      m_forwardRenderPipeline.execute(m_drawCommands, m_currentRenderTarget);
    else
      throw RuntimeErrorException("Frame Renderer: Not implemented render pipeline type.");

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
  }

  void FrameRenderer::destroy()
  {
    m_forwardRenderPipeline.destroy();
    m_deferredHybridRenderPipeline.destroy();
    m_lightFrameUBO.destroy();
    m_cameraFrameUBO.destroy();
    m_initialized = false;
  }

  void FrameRenderer::onViewportChanged(const Rect<UInt32>&viewportRect)
  {
    m_deferredHybridRenderPipeline.updateViewportSize(viewportRect);
  }

  void FrameRenderer::assertIsInitialized() const
  {
    if (!m_initialized)
      throw RuntimeErrorException("FrameRenderer is not initialized.");
  }
}
