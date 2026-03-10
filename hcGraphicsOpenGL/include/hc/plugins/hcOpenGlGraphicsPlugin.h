#pragma once

#include <hc/plugins/hcIGraphicsManagerPlugin.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  extern "C"
  {
    HC_GRAPHICS_OPENGL_EXPORT IPlugin* createGraphicsOpenGLPlugin();
    HC_GRAPHICS_OPENGL_EXPORT void destroyGraphicsOpenGLPlugin(IPlugin* plugin);
  }

  class OpenGlGraphicsManager;

  class HC_GRAPHICS_OPENGL_EXPORT OpenGlGraphicsPlugin : public IGraphicsManagerPlugin
  {
  public:
    OpenGlGraphicsPlugin();
    virtual ~OpenGlGraphicsPlugin() = default;

    /**
     * @copydoc IPlugin::onConnect
     */
    void onConnect() override;
    
    /**
     * @copydoc IPlugin::onClose
     */
    void onClose() override;

    /**
     * @copydoc IGraphicsManagerPlugin::createGraphicsManager
     */
    bool createGraphicsManager(
      IWindow& window,
      IAssetManager& assetManager,
      UniquePtr<MaterialFactoriesManager> materialFactoriesManager
    ) override;

    /**
     * @copydoc IGraphicsManagerPlugin::getGraphicsManager
     */
    IGraphicsManager& getGraphicsManager() override;

  private:
    OpenGlGraphicsManager* m_graphicsManager;
  };
}
