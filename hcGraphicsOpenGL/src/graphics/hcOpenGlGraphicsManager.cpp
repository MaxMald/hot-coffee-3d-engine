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
    m_queueDrawCommands(),
    m_viewportRect(0, 0, 1, 1),
    m_gBuffer(),
    m_polygonFillType(polygonFillType::Solid),
    m_renderPipelineType(renderPipelineType::Forward)
  {}

  OpenGlGraphicsManager::~OpenGlGraphicsManager()
  {}

  void OpenGlGraphicsManager::initialize(const Rect<UInt32>& viewportRect)
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
      m_gBuffer.clear();
  }

  void OpenGlGraphicsManager::draw(const DrawCommand& command)
  {
    m_queueDrawCommands.push_back(command);
  }

  void OpenGlGraphicsManager::executeDrawCommands()
  {
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
    if (renderPipelineType == renderPipelineType::DeferredHybrid)
    {
      if (!m_gBuffer.isValid())
        m_gBuffer.initialize(m_viewportRect.width, m_viewportRect.height);
      else
        m_gBuffer.resize(m_viewportRect.width, m_viewportRect.height);
    }

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
    glViewport(
      (GLint)viewportRect.x,
      (GLint)viewportRect.y,
      (GLsizei)viewportRect.width,
      (GLsizei)viewportRect.height
    );
    m_viewportRect = viewportRect;

    if (m_renderPipelineType == renderPipelineType::DeferredHybrid)
      m_gBuffer.resize(viewportRect.width, viewportRect.height);
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
    m_gBuffer.destroy();
  }

  void OpenGlGraphicsManager::executeForwardPass(
    const Vector<DrawCommand>& drawCommands
  )
  {
    for (const DrawCommand& command : drawCommands)
      executeDrawCommand(command);
  }

  void OpenGlGraphicsManager::executeDeferredGeometryPass(
    const Vector<DrawCommand>& drawCommands
  )
  { 
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

      m_gBuffer.bindForWriting();

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
        command.cameraMatrices,
        renderPassType::Type::DeferredGeometry
      );
      command.material->updateModelMatrix(command.modelMatrix);

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


    m_gBuffer.bindForReading();

    // TODO
    // This pass would typically involve rendering a full-screen quad and applying
    // lighting calculations in the shader using the G-buffer textures generated in the
    // geometry pass.

    m_gBuffer.unbind();
  }

  void OpenGlGraphicsManager::executeDeferredForwardPass(
    const Vector<DrawCommand>& drawCommands
  )
  {
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
      if (renderMode != materialRenderMode::Type::Transparent)
        return;

      executeDrawCommand(command);
    }
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

      command.material->bind(command.cameraMatrices, renderPassType::Type::Forward);
      command.material->updateModelMatrix(command.modelMatrix);

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
      command.material->bind(command.cameraMatrices, renderPassType::Type::Forward);
      command.material->updateModelMatrix(command.modelMatrix);

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
