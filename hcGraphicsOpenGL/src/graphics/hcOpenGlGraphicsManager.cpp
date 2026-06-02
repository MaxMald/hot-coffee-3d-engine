#include "hc/graphics/hcOpenGlGraphicsManager.h"

#include <GL/glew.h>
#include "hc/graphics/resource/texture/hcOpenGlTextureFactory.h"
#include "hc/graphics/resource/shader/hcOpenGlShaderFactory.h"
#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgramFactory.h"
#include "hc/graphics/resource/mesh/hcOpenGlMeshFactory.h"
#include "hc/graphics/hcDrawCommandUtilities.h"
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"
#include "hc/graphics/hcOpenGlFrameBuffer.h"

namespace hc
{
  static constexpr UInt32 LIGHTS_BINDING_POINT = 2;

  OpenGlGraphicsManager::OpenGlGraphicsManager(
    IWindow& window,
    IAssetManager& assetManager
  ) :
    m_window(window),
    m_assetManager(assetManager),
    m_textureManager(
      MakeUnique<OpenGlTextureFactory>(),
      m_assetManager
    ),
    m_shaderManager(
      MakeUnique<OpenGlShaderFactory>()
    ),
    m_shaderProgramManager(
      MakeUnique<OpenGlShaderProgramFactory>(),
      m_shaderManager
    ),
    m_materialManager(
      m_assetManager,
      m_textureManager,
      m_shaderProgramManager
    ),
    m_meshManager(
      m_assetManager,
      MakeUnique<OpenGlMeshFactory>(*this),
      m_materialManager
    ),
    m_currentCameraRenderData(),
    m_lightFrameUBO(),
    m_customRenderTarget(nullptr),
    m_queueDrawCommands(),
    m_viewportRect(0, 0, 1, 1),
    m_gBuffer(),
    m_deferredLightingShaderProgram(nullptr), // <- me quedé aquí
    m_polygonFillType(polygonFillType::Solid),
    m_renderPipelineType(renderPipelineType::Forward)
  {}

  OpenGlGraphicsManager::~OpenGlGraphicsManager()
  {}

  void OpenGlGraphicsManager::initialize(
    const GraphicsSettings& graphicsSettings,
    const Rect<UInt32>& viewportRect
  )
  {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
      throw RuntimeErrorException(
        "Failed to initialize GLEW: " +
        String(reinterpret_cast<const char*>(glewGetErrorString(err)))
      );
    }

    m_renderPipelineType = graphicsSettings.renderPipelineType;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    m_gBuffer.initialize(viewportRect.width, viewportRect.height);
    m_lightFrameUBO.initialize();
    m_materialManager.initialize();
    m_deferredLightingShaderProgram = m_shaderProgramManager.getBuiltInShaderProgram(
      builtInShaderProgramType::DeferredLighting
    );

    setViewport(viewportRect);
  }

  graphicsBackendType::Type OpenGlGraphicsManager::getGraphicsBackendType() const
  {
    return graphicsBackendType::OPENGL;
  }

  void OpenGlGraphicsManager::beginFrame()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_renderPipelineType == renderPipelineType::DeferredHybrid)
      m_gBuffer.clear(Color::Black()); // TODO - make this configurable
  }

  void OpenGlGraphicsManager::updateCameraRenderData(
    const CameraRenderData& cameraRenderData
  )
  {
    m_currentCameraRenderData = cameraRenderData;

    // TODO
    // 
    // Upload camera data to GPU via UBO
  }

  void OpenGlGraphicsManager::uploadLightFrameData(const LightFrameData& lightFrameData)
  {
    m_lightFrameUBO.upload(lightFrameData);
  }

  void OpenGlGraphicsManager::setRenderTarget(IFrameBuffer* frameBuffer)
  {
    if (frameBuffer)
    {
      if (!frameBuffer->isValid())
        throw InvalidArgumentException("Invalid framebuffer provided as render target.");

      frameBuffer->bind();
    }
    else
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    m_customRenderTarget = frameBuffer;
  }

  IFrameBuffer* OpenGlGraphicsManager::getRenderTarget() const
  {
    return m_customRenderTarget;
  }

  void OpenGlGraphicsManager::draw(const DrawCommand& command)
  {
    m_queueDrawCommands.push_back(command);
  }

  void OpenGlGraphicsManager::executeDrawCommands()
  {
    m_lightFrameUBO.bind(LIGHTS_BINDING_POINT);

    DrawCommandUtilities::SortDrawCommands(m_queueDrawCommands);

    if (m_renderPipelineType == renderPipelineType::Forward)
    {
      executeForwardPass(m_queueDrawCommands);
    }
    else if (m_renderPipelineType == renderPipelineType::DeferredHybrid)
    {
      m_deferredGeometryPassCommands.clear();
      m_deferredForwardPassCommands.clear();

      DrawCommandUtilities::SplitDrawCommandsByPipelinePath(
        m_queueDrawCommands,
        m_deferredGeometryPassCommands,
        m_deferredForwardPassCommands
      );

      executeDeferredGeometryPass(m_deferredGeometryPassCommands);
      executeDeferredLightingPass();
      executeDeferredForwardPass(m_deferredForwardPassCommands);
    }

    m_queueDrawCommands.clear();
  }

  void OpenGlGraphicsManager::endFrame(IWindow& window)
  {
    window.swapBuffers();
  }

  void OpenGlGraphicsManager::setPolygonFillType(polygonFillType::Type fillType)
  {
    glPolygonMode(
      GL_FRONT_AND_BACK,
      openGlGraphicsUtilities::GetOpenGlPolygonModeFromPolygonFillType(fillType)
    );
    m_polygonFillType = fillType;
  }

  polygonFillType::Type OpenGlGraphicsManager::getPolygonFillType() const
  {
    return m_polygonFillType;
  }

  void OpenGlGraphicsManager::setRenderPipelineType(
    renderPipelineType::Type renderPipelineType
  )
  {
    if (m_renderPipelineType == renderPipelineType)
      return;

    if (renderPipelineType == renderPipelineType::DeferredHybrid)
      m_gBuffer.resize(m_viewportRect.width, m_viewportRect.height);

    m_renderPipelineType = renderPipelineType;
  }

  renderPipelineType::Type OpenGlGraphicsManager::getRenderPipelineType() const
  {
    return m_renderPipelineType;
  }

  ITextureManager& OpenGlGraphicsManager::getTextureManager()
  {
    return m_textureManager;
  }

  IMaterialManager& OpenGlGraphicsManager::getMaterialManager()
  {
    return m_materialManager;
  }

  IShaderManager& OpenGlGraphicsManager::getShaderManager()
  {
    return m_shaderManager;
  }

  IShaderProgramManager& OpenGlGraphicsManager::getShaderProgramManager()
  {
    return m_shaderProgramManager;
  }

  IMeshManager& OpenGlGraphicsManager::getMeshManager()
  {
    return m_meshManager;
  }

  IGBuffer& OpenGlGraphicsManager::getGBuffer()
  {
    return m_gBuffer;
  }

  FrameBufferPtr OpenGlGraphicsManager::createFrameBuffer(
    UInt32 width,
    UInt32 height
  )
  {
    FrameBufferPtr frameBufferPtr = FrameBufferPtr(new OpenGlFrameBuffer());
    frameBufferPtr->initialize(width, height);
    return frameBufferPtr;
  }

  void OpenGlGraphicsManager::setViewport(const Rect<UInt32>& viewportRect)
  {
    if (viewportRect.width == 0 || viewportRect.height == 0)
      throw InvalidArgumentException("Viewport dimensions must be greater than zero.");

    if (viewportRect == m_viewportRect)
      return;

    glViewport(
      (GLint)viewportRect.x,
      (GLint)viewportRect.y,
      (GLsizei)viewportRect.width,
      (GLsizei)viewportRect.height
    );

    if (m_renderPipelineType == renderPipelineType::DeferredHybrid)
      m_gBuffer.resize(viewportRect.width, viewportRect.height);

    m_viewportRect = viewportRect;
  }

  Rect<UInt32> OpenGlGraphicsManager::getViewportRect() const
  {
    return m_viewportRect;
  }

  void OpenGlGraphicsManager::destroy()
  {
    m_materialManager.clear();
    m_textureManager.clear();
    m_shaderProgramManager.clear();
    m_shaderManager.clear();
    m_meshManager.clear();
    m_gBuffer.cleanup();
    m_lightFrameUBO.destroy();
  }

  void OpenGlGraphicsManager::executeForwardPass(
    const Vector<DrawCommand>& drawCommands
  )
  {
    if (m_customRenderTarget)
    {
      if (!m_customRenderTarget->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");
      m_customRenderTarget->bind();
    }

    for (const DrawCommand& command : drawCommands)
      executeDrawCommand(command);

    if (m_customRenderTarget)
      m_customRenderTarget->unbind();
  }

  void OpenGlGraphicsManager::executeDeferredGeometryPass(
    const Vector<DrawCommand>& drawCommands
  )
  {
    m_gBuffer.bind();

    for (const DrawCommand& command : drawCommands)
    {
      String errorMessage;
      if (!isValidDrawCommand(command, errorMessage))
      {
        LogService::Error(
          "Invalid draw command, skipping execution: " + errorMessage
        );
        continue;
      }

      materialRenderMode::Type renderMode = command.material->getRenderMode();
      if (renderMode != materialRenderMode::Type::AlphaCutout
        && renderMode != materialRenderMode::Type::Opaque)
        continue;

      bool isTwoSided = command.material->isDoubleSided();
      if (isTwoSided)
        glDisable(GL_CULL_FACE);

      const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);
      glBindVertexArray(drawData.vao);

      command.material->bind(
        command.cameraRenderData,
        renderPassType::Type::DeferredGeometry
      );
      command.material->updateModelMatrix(
        command.modelMatrix,
        renderPassType::Type::DeferredGeometry
      );

      glDrawElements(
        drawData.drawMode,
        static_cast<GLsizei>(command.indexCount),
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
      );

      command.material->unbind();
      glBindVertexArray(0);

      if (isTwoSided)
        glEnable(GL_CULL_FACE);
    }

    m_gBuffer.unbind();
  }

  void OpenGlGraphicsManager::executeDeferredLightingPass()
  {
    if (m_customRenderTarget)
    {
      if (!m_customRenderTarget->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");
      m_customRenderTarget->bind();
    }

    m_deferredLightingShaderProgram->bind();
    m_deferredLightingShaderProgram->setUniform("uCameraPosition", m_currentCameraRenderData.cameraWorldPosition);
    m_gBuffer.bindGTexturesForReading();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_gBuffer.unbind();

    if (m_customRenderTarget)
      m_customRenderTarget->unbind();
  }

  void OpenGlGraphicsManager::executeDeferredForwardPass(
    const Vector<DrawCommand>& drawCommands
  )
  {
    if (m_customRenderTarget)
    {
      if (!m_customRenderTarget->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");

      m_gBuffer.copyDepthTo(*m_customRenderTarget);
      m_customRenderTarget->bind();
    }
    else
    {
      m_gBuffer.bindForReadingOnly();
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Bind default framebuffer for drawing
      glBlitFramebuffer(
        0, 0, m_gBuffer.getWidth(), m_gBuffer.getHeight(),
        0, 0, m_viewportRect.width, m_viewportRect.height,
        GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
      );
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    for (const DrawCommand& command : drawCommands)
    {
      String errorMessage;
      if (!isValidDrawCommand(command, errorMessage))
      {
        LogService::Error(
          "Invalid draw command, skipping execution: " + errorMessage
        );
        continue;
      }

      executeDrawCommand(command);
    }

    if (m_customRenderTarget)
      m_customRenderTarget->unbind();
  }

  void OpenGlGraphicsManager::executeDrawCommand(
    const DrawCommand& command
  )
  {
    String errorMessage;
    if (!isValidDrawCommand(command, errorMessage))
    {
      LogService::Error(
        "Invalid draw command, skipping execution: " + errorMessage
      );

      return;
    }

    const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

    bool isTwoSided = command.material->isDoubleSided();
    bool isTransparent = command.material->getRenderMode() == materialRenderMode::Type::Transparent;

    if (isTransparent)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDepthMask(GL_FALSE);
    }

    if (isTwoSided && isTransparent)
    {
      glBindVertexArray(drawData.vao);

      command.material->bind(command.cameraRenderData, renderPassType::Type::Forward);
      command.material->updateModelMatrix(command.modelMatrix, renderPassType::Type::Forward);

      // Pass 1: Render back faces first
      glCullFace(GL_FRONT);
      glDrawElements(
        drawData.drawMode,
        static_cast<GLsizei>(command.indexCount),
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
      );

      // Pass 2: Render front faces
      glCullFace(GL_BACK);
      glDrawElements(
        drawData.drawMode,
        static_cast<GLsizei>(command.indexCount),
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
      );

      command.material->unbind();
      glBindVertexArray(0);
    }
    else
    {
      if (isTwoSided)
        glDisable(GL_CULL_FACE);

      glBindVertexArray(drawData.vao);
      command.material->bind(command.cameraRenderData, renderPassType::Type::Forward);
      command.material->updateModelMatrix(command.modelMatrix, renderPassType::Type::Forward);

      glDrawElements(
        drawData.drawMode,
        static_cast<GLsizei>(command.indexCount),
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
      );

      command.material->unbind();
      glBindVertexArray(0);

      if (isTwoSided)
        glEnable(GL_CULL_FACE);
    }

    if (isTransparent)
    {
      glDepthMask(GL_TRUE);
      glDisable(GL_BLEND);
    }
  }

  bool OpenGlGraphicsManager::isValidDrawCommand(
    const DrawCommand& drawCommand,
    String& errorMessage
  )
  {
    if (!drawCommand.material)
    {
      errorMessage = "Draw command has no material assigned.";
      return false;
    }
    else if (!std::holds_alternative<OpenGlDrawData>(drawCommand.apiDrawData))
    {
      errorMessage = "Draw command has invalid API draw data type, expected OpenGlDrawData.";
      return false;
    }
    else
    {
      const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(drawCommand.apiDrawData);
      if (drawData.vao == 0)
      {
        errorMessage = "Draw command has invalid VAO (0).";
        return false;
      }
    }
    return true;
  }
}
