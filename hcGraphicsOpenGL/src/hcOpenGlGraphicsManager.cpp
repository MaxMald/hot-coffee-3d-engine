#include "hc/hcOpenGlGraphicsManager.h"

#include <GL/glew.h>
#include <hc/hcTextureManager.h>
#include <hc/hcShaderManager.h>
#include <hc/hcShaderProgramManager.h>
#include <hc/hcMaterialManager.h>
#include <hc/hcMaterialFactoriesManagerRegistry.h>
#include <hc/hcMaterialFactoriesManager.h>
#include <hc/hcMeshManager.h>

#include "hc/hcOpenGlTextureFactory.h"
#include "hc/hcOpenGlShaderFactory.h"
#include "hc/hcOpenGlShaderProgramFactory.h"
#include "hc/hcOpenGlMeshFactory.h"

namespace hc
{
  OpenGlGraphicsManager::OpenGlGraphicsManager() :
    m_assetManager(nullptr),
    m_sceneManager(nullptr)
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
    if (!m_textureManager)
    {
      throw RuntimeErrorException(
        "Texture manager is not initialized."
      );
    }

    return *m_textureManager;
  }

  IMaterialManager& OpenGlGraphicsManager::getMaterialManager()
  {
    if (!m_materialManager)
    {
      throw RuntimeErrorException(
        "Material manager is not initialized."
      );
    }

    return *m_materialManager;
  }

  IShaderManager& OpenGlGraphicsManager::getShaderManager()
  {
    if (!m_shaderManager)
    {
      throw RuntimeErrorException(
        "Shader manager is not initialized."
      );
    }

    return *m_shaderManager;
  }

  IShaderProgramManager& OpenGlGraphicsManager::getShaderProgramManager()
  {
    if (!m_shaderProgramManager)
    {
      throw RuntimeErrorException(
        "Shader program manager is not initialized."
      );
    }

    return *m_shaderProgramManager;
  }

  IMeshManager& OpenGlGraphicsManager::getMeshManager()
  {
    if (!m_meshManager)
    {
      throw RuntimeErrorException(
        "Mesh manager is not initialized."
      );
    }

    return *m_meshManager;
  }

  void OpenGlGraphicsManager::initialize(
    IWindow& window,
    AssetManager& assetManager
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
    m_textureManager = MakeUnique<TextureManager>(
      MakeUnique<OpenGlTextureFactory>(),
      *m_assetManager
    );

    m_shaderManager = MakeUnique<ShaderManager>(
      MakeUnique<OpenGlShaderFactory>()
    );

    m_shaderProgramManager = MakeUnique<ShaderProgramManager>(
      MakeUnique<OpenGlShaderProgramFactory>(),
      *m_shaderManager
    );

    UniquePtr<MaterialFactoriesManager> materialFactoriesManager =
      MakeUnique<MaterialFactoriesManager>();

    materialFactoriesManagerRegistry::resigtryMaterialFactories(
      *materialFactoriesManager
    );

    m_materialManager = MakeUnique<MaterialManager>(
      *m_assetManager,
      *m_textureManager,
      *m_shaderProgramManager,
      std::move(materialFactoriesManager)
    );

    m_meshManager = MakeUnique<MeshManager>(
      *m_assetManager,
      MakeUnique<OpenGlMeshFactory>(),
      *m_materialManager
    );
  }
}
