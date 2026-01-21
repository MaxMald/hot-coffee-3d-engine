#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/hcIGraphicsManager.h"

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
