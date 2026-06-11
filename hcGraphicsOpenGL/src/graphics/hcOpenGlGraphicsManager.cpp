#include "hc/graphics/hcOpenGlGraphicsManager.h"

#include <GL/glew.h>
#include "hc/graphics/resource/texture/hcOpenGlTextureFactory.h"
#include "hc/graphics/resource/shader/hcOpenGlShaderFactory.h"
#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgramFactory.h"
#include "hc/graphics/resource/mesh/hcOpenGlMeshFactory.h"
#include "hc/graphics/resource/frameBuffer/hcOpenGlFrameBuffer.h"
#include "hc/graphics/cubeMap/hcOpenGlCubeMap.h"
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
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
    m_viewportRect(0, 0, 1, 1),
    m_frameRenderer(),
    m_polygonFillType(polygonFillType::Solid)
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
    m_frameRenderer.initialize(viewportRect, m_shaderProgramManager);
    m_frameRenderer.setRenderPipeline(graphicsSettings.renderPipelineType);
    setViewport(viewportRect);
  }

  graphicsBackendType::Type OpenGlGraphicsManager::getGraphicsBackendType() const
  {
    return graphicsBackendType::OPENGL;
  }

  void OpenGlGraphicsManager::beginFrame()
  {
    m_frameRenderer.clearFrame();
  }

  void OpenGlGraphicsManager::uploadCameraFrameData(
    const CameraFrameData& cameraFrameData
  )
  {
    m_frameRenderer.uploadCameraFrameData(cameraFrameData);
  }

  void OpenGlGraphicsManager::uploadLightFrameData(const LightFrameData& lightFrameData)
  {
    m_frameRenderer.uploadLightFrameData(lightFrameData);
  }

  void OpenGlGraphicsManager::setRenderTarget(IFrameBuffer* frameBuffer)
  {
    m_frameRenderer.setRenderTarget(frameBuffer);
  }

  IFrameBuffer* OpenGlGraphicsManager::getRenderTarget() const
  {
    return m_frameRenderer.getRenderTarget();
  }

  void OpenGlGraphicsManager::setSkybox(ICubeMap* skyboxCubeMap)
  {
    if (skyboxCubeMap)
      m_frameRenderer.setSkybox(reinterpret_cast<OpenGlCubeMap*>(skyboxCubeMap));
    else
      m_frameRenderer.removeSkybox();
  }

  void OpenGlGraphicsManager::draw(const DrawCommand& command)
  {
    m_frameRenderer.queueDrawCommand(command);
  }

  void OpenGlGraphicsManager::executeDrawCommands()
  {
    m_frameRenderer.execute();
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
    m_frameRenderer.setRenderPipeline(renderPipelineType);
  }

  renderPipelineType::Type OpenGlGraphicsManager::getRenderPipelineType() const
  {
    return m_frameRenderer.getCurrentRenderPipelineType();
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
    return m_frameRenderer.getGBuffer();
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

    m_frameRenderer.onViewportChanged(viewportRect);
    m_viewportRect = viewportRect;
  }

  Rect<UInt32> OpenGlGraphicsManager::getViewportRect() const
  {
    return m_viewportRect;
  }

  void OpenGlGraphicsManager::destroy()
  {
    m_frameRenderer.destroy();
    m_materialManager.clear();
    m_textureManager.clear();
    m_shaderProgramManager.clear();
    m_shaderManager.clear();
    m_meshManager.clear();
  }
}
