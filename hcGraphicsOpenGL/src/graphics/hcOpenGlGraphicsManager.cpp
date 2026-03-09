#include "hc/graphics/hcOpenGlGraphicsManager.h"

#include <GL/glew.h>
#include <hc/graphics/resource/material/hcMaterialFactoriesManagerRegistry.h>
#include <hc/graphics/resource/material/hcMaterialFactoriesManager.h>

#include "hc/graphics/resource/texture/hcOpenGlTextureFactory.h"
#include "hc/graphics/resource/shader/hcOpenGlShaderFactory.h"
#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgramFactory.h"
#include "hc/graphics/resource/mesh/hcOpenGlMeshFactory.h"

namespace hc
{
  OpenGlGraphicsManager::OpenGlGraphicsManager(
    IWindow& window,
    IAssetManager& assetManager,
    UniquePtr<MaterialFactoriesManager> materialFactoriesManager
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
      std::move(materialFactoriesManager)
    ),
    m_meshManager(
      m_assetManager,
      MakeUnique<OpenGlMeshFactory>(),
      m_materialManager
    )
  {
  }

  OpenGlGraphicsManager::~OpenGlGraphicsManager()
  {
  }

  void OpenGlGraphicsManager::beginFrame()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
}
