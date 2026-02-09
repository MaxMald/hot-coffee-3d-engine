#include "hc/hcOpenGlGraphicsManager.h"

#include <GL/glew.h>
#include "hc/hcOpenGlShader.h"
#include "hc/hcOpenGlShaderProgram.h"
#include "hc/hcBuiltInShaders.h"
#include "hc/hcOpenGlTexture.h"

namespace hc
{
  OpenGlGraphicsManager::OpenGlGraphicsManager()
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

  void OpenGlGraphicsManager::resolveDependencies(DependencyContainer& container)
  {
    m_assetManager = container.resolve<AssetManager>();
  }

  void OpenGlGraphicsManager::init(IWindow& window)
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
      static_cast<GLsizei>(window.getSize().x),
      static_cast<GLsizei>(window.getSize().y)
    );

    prepareManagers();
  }

  void OpenGlGraphicsManager::destroy()
  {
  }

  void OpenGlGraphicsManager::prepareManagers()
  {
    m_textureManager.initialize(
      m_assetManager
    );
    m_shaderProgramManager.initialize(
      &m_shaderManager
    );
    m_materialManager.initialize(
      m_assetManager,
      &m_textureManager,
      &m_shaderProgramManager
    );
    m_meshManager.initialize(
      m_assetManager,
      &m_materialManager
    );
  }
}
