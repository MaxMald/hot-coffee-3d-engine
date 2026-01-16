#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/hcIPlugin.h"

namespace hc
{
  extern "C"
  {
    HC_GRAPHICS_OPENGL_EXPORT IPlugin* createGraphicsOpenGLPlugin();
    HC_GRAPHICS_OPENGL_EXPORT void destroyGraphicsOpenGLPlugin();
  }

  class HC_GRAPHICS_OPENGL_EXPORT OpenGlGraphicsPlugin : public IPlugin
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
    * @copydoc IPlugin::getData
    */
    void* getData() override;

    /**
    * @copydoc IPlugin::addDependencies
    */
    void addDependencies(DependencyContainer& container) override;
  };
}
