#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/hcOpenGlTextureManager.h"
#include "hc/hcOpenGlMaterialManager.h"
#include "hc/hcOpenGlShaderManager.h"
#include "hc/hcOpenGlShaderProgramManager.h"
#include "hc/hcOpenGlMeshManager.h"

namespace hc
{
  class HC_GRAPHICS_OPENGL_EXPORT OpenGlGraphicsManager :
    public IGraphicsManager,
    public IDependencyResolvable
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

    /**
     * @copydoc IDependencyResolvable::resolveDependencies
     */
    void resolveDependencies(DependencyContainer& container) override;

  private:
    SharedPtr<AssetManager> m_assetManager;
    SharedPtr<SceneManager> m_sceneManager;
    OpenGlTextureManager m_textureManager;
    OpenGlMaterialManager m_materialManager;
    OpenGlShaderManager m_shaderManager;
    OpenGlShaderProgramManager m_shaderProgramManager;
    OpenGlMeshManager m_meshManager;

    /**
     * @copydoc IGraphicsManager::init
     */
    void init() override;

    /**
     * @copydoc IGraphicsManager::destroy
     */
    void destroy() override;

    void prepareManagers();
  };
}
