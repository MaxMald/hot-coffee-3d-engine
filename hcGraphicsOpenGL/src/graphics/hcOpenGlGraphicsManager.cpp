#include "hc/graphics/hcOpenGlGraphicsManager.h"

#include <GL/glew.h>
#include <hc/graphics/resource/material/hcMaterialFactoriesManager.h>
#include <hc/graphics/resource/material/hcMaterialFactoriesManagerFactory.h>
#include "hc/graphics/resource/texture/hcOpenGlTextureFactory.h"
#include "hc/graphics/resource/shader/hcOpenGlShaderFactory.h"
#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgramFactory.h"
#include "hc/graphics/resource/mesh/hcOpenGlMeshFactory.h"
#include "hc/graphics/hcDrawCommandUtilities.h"
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
      m_shaderProgramManager,
      MaterialFactoriesManagerFactory::Create()
    ),
    m_meshManager(
      m_assetManager,
      MakeUnique<OpenGlMeshFactory>(*this),
      m_materialManager
    ),
    m_drawCommands()
  {
  }

  OpenGlGraphicsManager::~OpenGlGraphicsManager()
  {
  }

  graphicsBackendType::Type OpenGlGraphicsManager::getGraphicsBackendType() const
  {
    return graphicsBackendType::OPENGL;
  }

  void OpenGlGraphicsManager::beginFrame()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGlGraphicsManager::draw(const DrawCommand& command)
  {
    m_drawCommands.push_back(command);
  }

  void OpenGlGraphicsManager::executeDrawCommands()
  {
    DrawCommandUtilities::SortDrawCommands(m_drawCommands);

    for (const DrawCommand& command : m_drawCommands)
      executeDrawCommand(command);

    m_drawCommands.clear();
  }

  void OpenGlGraphicsManager::endFrame(IWindow& window)
  {
    window.swapBuffers();
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

  FrameBufferPtr OpenGlGraphicsManager::createFrameBuffer(
    UInt32 width,
    UInt32 height
  )
  {
    FrameBufferPtr frameBufferPtr = FrameBufferPtr(new OpenGlFrameBuffer());
    frameBufferPtr->initialize(width, height);
    return frameBufferPtr;
  }

  void OpenGlGraphicsManager::setViewport(
    UInt32 x,
    UInt32 y,
    UInt32 width,
    UInt32 height
  )
  {
    glViewport(x, y, width, height);
  }

  void OpenGlGraphicsManager::initialize()
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
    glViewport(
      0, 0,
      static_cast<GLsizei>(m_window.getSize().x),
      static_cast<GLsizei>(m_window.getSize().y)
    );
  }

  void OpenGlGraphicsManager::destroy()
  {
    m_materialManager.clear();
    m_textureManager.clear();
    m_shaderProgramManager.clear();
    m_shaderManager.clear();
    m_meshManager.clear();
  }

  void OpenGlGraphicsManager::executeDrawCommand(
    const DrawCommand& command
  )
  {
    if (!command.material)
    {
      LogService::Error(
        "Draw command has no material assigned, skipping draw call."
      );
      return;
    }

    SharedPtr<AMaterialDescriptor> descriptor = command.material->getDescriptor();
    if (!descriptor)
    {
      LogService::Error(
        "Draw command's material has no descriptor, skipping draw call."
      );
      return;
    }

    if (!std::holds_alternative<OpenGlDrawData>(command.apiDrawData))
    {
      LogService::Error(
        "Draw command does not contain OpenGL draw data, skipping draw call."
      );
      return;
    }

    const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);
    if (drawData.vao == 0)
    {
      LogService::Error(
        "Draw command has invalid VAO (0), skipping draw call."
      );
      return;
    }

    bool isTwoSided = descriptor->isDoubleSided();
    bool isTransparent = command.material->isTransparent();

    if (isTransparent)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    if (isTwoSided && isTransparent)
    {
      glDepthMask(GL_FALSE);
      glBindVertexArray(drawData.vao);

      command.material->bind(command.cameraMatrices);
      command.material->updateModelMatrix(command.modelMatrix);

      // Pass 1: Render back faces first
      glCullFace(GL_FRONT);
      glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(command.indexCount),
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
      );

      // Pass 2: Render front faces
      glCullFace(GL_BACK);
      glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(command.indexCount),
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
      );

      command.material->unbind();
      glBindVertexArray(0);
      glDepthMask(GL_TRUE);
    }
    else
    {
      if (isTwoSided)
        glDisable(GL_CULL_FACE);

      if (isTransparent)
        glDepthMask(GL_FALSE);

      glBindVertexArray(drawData.vao);
      command.material->bind(command.cameraMatrices);
      command.material->updateModelMatrix(command.modelMatrix);

      glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(command.indexCount),
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
      );

      command.material->unbind();
      glBindVertexArray(0);

      if (isTransparent)
      {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
      } 

      if (isTwoSided)
        glEnable(GL_CULL_FACE);
    }   
  }
}
