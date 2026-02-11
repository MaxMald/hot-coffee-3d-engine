#include "hc/hcWindowSfmlPlugin.h"
#include "hc/hcSfmlWindowManager.h"

namespace hc
{
  HC_WINDOW_SFML_EXPORT IPlugin* createWindowSfmlPlugin()
  {
    return new WindowSfmlPlugin();
  }

  HC_WINDOW_SFML_EXPORT void destroyWindowSfmlPlugin()
  {
  }

  WindowSfmlPlugin::WindowSfmlPlugin()
  {
  }

  WindowSfmlPlugin::~WindowSfmlPlugin()
  {
  }

  void WindowSfmlPlugin::onConnect()
  {
    LogService::Message("hcWindowSfmlPlugin connected.");
  }

  void WindowSfmlPlugin::onClose()
  {
    LogService::Message("hcWindowSfmlPlugin closed.");
  }

  UniquePtr<IWindowManager> WindowSfmlPlugin::createWindowManager() const
  {
    return MakeUnique<SfmlWindowManager>();
  }
}
