#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class WindowManager;

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
     * @copydoc IGraphicsManager::draw
     */
    void draw(IDrawable&) override;

    /**
     * @copydoc IGraphicsManager::endFrame
     */
    void endFrame(IWindow&) override;

    /**
     * @copydoc IGraphicsManager::createTexture
     */
    SharedPtr<ITexture> createTexture(SharedPtr<Image> image) const override;

    /**
     * @copydoc IGraphicsManager::createShaderFromString
     */
    SharedPtr<IShader> createShaderFromString(
      shaderStageType::Type stageType,
      const String& shaderCode
    ) const override;

    /**
     * @copydoc IGraphicsManager::createUnlitShaderProgram
     */
    SharedPtr<IShaderProgram> createUnlitShaderProgram() const override;

  private:
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
