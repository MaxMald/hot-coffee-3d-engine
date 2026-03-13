#include "hc/plugins/hcOpenGlGraphicsPlugin.h"
#include "hc/graphics/hcOpenGlGraphicsManager.h"

namespace hc
{
  HC_GRAPHICS_OPENGL_EXPORT IPlugin* createGraphicsOpenGLPlugin()
  {
    return new OpenGlGraphicsPlugin();
  }

  HC_GRAPHICS_OPENGL_EXPORT void destroyGraphicsOpenGLPlugin(IPlugin* plugin)
  {
    delete plugin;
  }

  OpenGlGraphicsPlugin::OpenGlGraphicsPlugin() :
    m_graphicsManager(nullptr)
  {
  }

  void OpenGlGraphicsPlugin::onConnect()
  {
  }

  void OpenGlGraphicsPlugin::onClose()
  {
    if (m_graphicsManager)
    {
      m_graphicsManager->destroy();
      delete m_graphicsManager;
      m_graphicsManager = nullptr;
    }
  }

  IGraphicsManager& OpenGlGraphicsPlugin::getGraphicsManager()
  {
    if (m_graphicsManager == nullptr)
      throw RuntimeErrorException(
        "Graphics manager has not been created yet. Call createGraphicsManager() before accessing the graphics manager instance."
      );

    return *m_graphicsManager;
  }

  bool OpenGlGraphicsPlugin::createGraphicsManager(
    IWindow& window,
    IAssetManager& assetManager
  )
  {
    try
    {
      if (m_graphicsManager)
        return false;

      m_graphicsManager = new OpenGlGraphicsManager(window, assetManager);
      return true;
    }
    catch (const Exception&)
    {
      return false;
    }
  }
}
