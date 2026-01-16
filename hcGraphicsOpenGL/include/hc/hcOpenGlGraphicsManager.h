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
     * @copydoc IGraphicsManager::init
     */
    void init() override;

    /**
     * @copydoc IGraphicsManager::draw
     */
    void draw(IWindow&) override;

    /**
     * @copydoc IGraphicsManager::destroy
     */
    void destroy() override;

  private:
    SharedPtr<IWindow> m_window;
  };
}
