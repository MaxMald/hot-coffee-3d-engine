#include "hc/plugins/hcWindowSfmlPlugin.h"
#include "hc/window/hcSfmlWindowManager.h"

namespace hc
{
  HC_WINDOW_SFML_EXPORT IPlugin* createWindowSfmlPlugin()
  {
    return new WindowSfmlPlugin();
  }

  HC_WINDOW_SFML_EXPORT void destroyWindowSfmlPlugin(IPlugin* plugin)
  {
    delete plugin;
  }

  WindowSfmlPlugin::WindowSfmlPlugin() :
    m_windowManager()
  {
  }

  WindowSfmlPlugin::~WindowSfmlPlugin()
  {
  }

  void WindowSfmlPlugin::onConnect()
  {
  }

  void WindowSfmlPlugin::onClose()
  {
    m_windowManager.destroy();
  }

  IWindowManager& WindowSfmlPlugin::getWindowManager()
  {
    return m_windowManager;
  }
}
