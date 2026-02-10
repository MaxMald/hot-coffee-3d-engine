#pragma once

#include <hc/hcIGraphicsManagerPlugin.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  extern "C"
  {
    HC_GRAPHICS_OPENGL_EXPORT IPlugin* createGraphicsOpenGLPlugin();
    HC_GRAPHICS_OPENGL_EXPORT void destroyGraphicsOpenGLPlugin();
  }

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
    * @copydoc IPlugin::addDependencies
    */
    void addDependencies(DependencyContainer& container) override;

    /**
     * @copydoc IGraphicsManagerPlugin::createGraphicsManager
     */
    UniquePtr<IGraphicsManager> createGraphicsManager(
      IWindow& window,
      AssetManager& assetManager,
      UniquePtr<MaterialFactoriesManager> materialFactoriesManager
    ) const override;
  };
}
