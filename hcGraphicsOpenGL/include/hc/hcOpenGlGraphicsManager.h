#pragma once

#include <hc/hcTextureManager.h>
#include <hc/hcMeshManager.h>
#include <hc/hcMaterialManager.h>
#include <hc/hcShaderManager.h>
#include <hc/hcShaderProgramManager.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"


namespace hc
{
  class HC_GRAPHICS_OPENGL_EXPORT OpenGlGraphicsManager :
    public IGraphicsManager
  {
  public:
    OpenGlGraphicsManager(
      IWindow& window,
      AssetManager& assetManager,
      UniquePtr<MaterialFactoriesManager> materialFactoriesManager
    );
    virtual ~OpenGlGraphicsManager();

    /**
     * @copydoc IGraphicsManager::beginFrame
     */
    void beginFrame() override;

    /**
     * @copydoc IGraphicsManager::endFrame
     */
    void endFrame(IWindow&) override; 

    /**
     * @copydoc IGraphicsManager::getTextureManager
     */
    ITextureManager& getTextureManager() override;

    /**
     * @copydoc IGraphicsManager::getMaterialManager
     */
    IMaterialManager& getMaterialManager() override;

    /**
     * @copydoc IGraphicsManager::getShaderManager
     */
    IShaderManager& getShaderManager() override;

    /**
     * @copydoc IGraphicsManager::getShaderProgramManager
     */
    IShaderProgramManager& getShaderProgramManager() override;

    /**
     * @copydoc IGraphicsManager::getMeshManager
     */
    IMeshManager& getMeshManager() override;

  private:
    AssetManager& m_assetManager;
    IWindow& m_window;
    TextureManager m_textureManager;
    ShaderManager m_shaderManager;
    ShaderProgramManager m_shaderProgramManager;
    MaterialManager m_materialManager;
    MeshManager m_meshManager;

    /**
     * @copydoc IGraphicsManager::initialize
     */
    void initialize() override;

    /**
     * @copydoc IGraphicsManager::destroy
     */
    void destroy() override;
  };
}
