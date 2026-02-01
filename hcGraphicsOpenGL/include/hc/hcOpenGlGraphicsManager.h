#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/hcOpenGlTextureManager.h"
#include "hc/hcOpenGlMaterialManager.h"
#include "hc/hcOpenGlShaderManager.h"

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
     * @copydoc IGraphicsManager::draw
     */
    void draw(IDrawable&) override;

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
     * @copydoc IGraphicsManager::createUnlitShaderProgram
     */
    SharedPtr<IShaderProgram> createUnlitShaderProgram() const override;

    /**
     * @copydoc IDependencyResolvable::resolveDependencies
     */
    void resolveDependencies(DependencyContainer& container) override;

  private:
    OpenGlTextureManager m_textureManager;
    OpenGlMaterialManager m_materialManager;
    OpenGlShaderManager m_shaderManager;

    /**
     * @copydoc IGraphicsManager::init
     */
    void init() override;

    /**
     * @copydoc IGraphicsManager::destroy
     */
    void destroy() override;
  };
}
