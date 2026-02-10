#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class HC_GRAPHICS_OPENGL_EXPORT OpenGlGraphicsManager :
    public IGraphicsManager
  {
  public:
    OpenGlGraphicsManager();
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
    AssetManager* m_assetManager;

    UniquePtr<ITextureManager> m_textureManager;
    UniquePtr<IMaterialManager> m_materialManager;
    UniquePtr<IShaderManager> m_shaderManager;
    UniquePtr<IShaderProgramManager> m_shaderProgramManager;
    UniquePtr<IMeshManager> m_meshManager;

    /**
     * @copydoc IGraphicsManager::initialize
     */
    void initialize(
      IWindow& window,
      AssetManager& assetManager
    ) override;

    /**
     * @copydoc IGraphicsManager::destroy
     */
    void destroy() override;

    void prepareManagers();
  };
}
