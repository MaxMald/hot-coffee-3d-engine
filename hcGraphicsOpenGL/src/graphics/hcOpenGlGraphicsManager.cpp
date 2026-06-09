#include "hc/graphics/hcOpenGlGraphicsManager.h"

#include <GL/glew.h>
#include "hc/graphics/resource/texture/hcOpenGlTextureFactory.h"
#include "hc/graphics/resource/shader/hcOpenGlShaderFactory.h"
#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgramFactory.h"
#include "hc/graphics/resource/mesh/hcOpenGlMeshFactory.h"
#include "hc/graphics/resource/frameBuffer/hcOpenGlFrameBuffer.h"
#include "hc/graphics/cubeMap/hcOpenGlCubeMap.h"
#include "hc/graphics/hcDrawCommandUtilities.h"
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  static constexpr UInt32 CAMERA_FRAME_BINDING_POINT = 1;
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
    m_lightFrameUBO(),
    m_cameraFrameUBO(),
    m_customRenderTarget(nullptr),
    m_queueDrawCommands(),
    m_viewportRect(0, 0, 1, 1),
    m_forwardRenderPipeline(),
    m_deferredHybridRenderPipeline(m_forwardRenderPipeline),
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    m_materialManager.initialize();
    setViewport(viewportRect);

    m_renderPipelineType = graphicsSettings.renderPipelineType;
    m_deferredHybridRenderPipeline.initialize(
      viewportRect,
      m_shaderProgramManager.getBuiltInShaderProgram(builtInShaderProgramType::DeferredLighting)
    );

    m_lightFrameUBO.initialize(LightFrameData{});
    m_cameraFrameUBO.initialize(CameraFrameData{});
    m_cameraFrameUBO.bindBase(CAMERA_FRAME_BINDING_POINT);
    m_lightFrameUBO.bindBase(LIGHTS_BINDING_POINT);
  }

  graphicsBackendType::Type OpenGlGraphicsManager::getGraphicsBackendType() const
  {
    return graphicsBackendType::OPENGL;
  }

  void OpenGlGraphicsManager::beginFrame()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_renderPipelineType == renderPipelineType::DeferredHybrid)
      m_deferredHybridRenderPipeline.clearGBuffer();
  }

  void OpenGlGraphicsManager::uploadCameraFrameData(
    const CameraFrameData& cameraFrameData
  )
  {
    CameraFrameData transposedCameraData = cameraFrameData;
    transposedCameraData.projectionMatrix.transpose();
    transposedCameraData.viewMatrix.transpose();
    m_cameraFrameUBO.upload(transposedCameraData);
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
    String errorMessage;
    if (!isValidDrawCommand(command, errorMessage))
    {
      LogService::Error("Invalid draw command, it will be discarded. Error: " + errorMessage);
      return;
    }

    m_queueDrawCommands.push_back(command);
  }

  void OpenGlGraphicsManager::executeDrawCommands()
  {
    DrawCommandUtilities::SortDrawCommands(m_queueDrawCommands);

    if (m_renderPipelineType == renderPipelineType::Forward)
    {
      m_forwardRenderPipeline.executeDrawCommands(
        m_queueDrawCommands,
        m_customRenderTarget
      );
    }
    else if (m_renderPipelineType == renderPipelineType::DeferredHybrid)
    {
      m_deferredHybridRenderPipeline.executeDrawCommands(
        m_queueDrawCommands,
        m_customRenderTarget
      );
    }
    else
    {
      throw RuntimeErrorException("Unsupported render pipeline type set in graphics manager.");
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
      m_deferredHybridRenderPipeline.updateViewportSize(m_viewportRect);

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
    return m_deferredHybridRenderPipeline.getGBuffer();
  }

  FrameBufferPtr OpenGlGraphicsManager::createFrameBuffer(UInt32 width, UInt32 height)
  {
    FrameBufferPtr frameBufferPtr = FrameBufferPtr(new OpenGlFrameBuffer());
    frameBufferPtr->initialize(width, height);
    return frameBufferPtr;
  }

  SharedPtr<ICubeMap> OpenGlGraphicsManager::createCubeMap()
  {
    return MakeShared<OpenGlCubeMap>();
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
      m_deferredHybridRenderPipeline.updateViewportSize(viewportRect);

    m_viewportRect = viewportRect;
  }

  Rect<UInt32> OpenGlGraphicsManager::getViewportRect() const
  {
    return m_viewportRect;
  }

  void OpenGlGraphicsManager::destroy()
  {
    m_deferredHybridRenderPipeline.destroy();
    m_materialManager.clear();
    m_textureManager.clear();
    m_shaderProgramManager.clear();
    m_shaderManager.clear();
    m_meshManager.clear();
    m_lightFrameUBO.destroy();
    m_cameraFrameUBO.destroy();
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
