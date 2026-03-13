#pragma once

#include <hc/plugins/hcIWindowManagerPlugin.h>
#include "hc/hcWindowSfmlPrerequisites.h"
#include "hc/window/hcSfmlWindowManager.h"

namespace hc
{
  extern "C"
  {
    HC_WINDOW_SFML_EXPORT IPlugin* createWindowSfmlPlugin();
    HC_WINDOW_SFML_EXPORT void destroyWindowSfmlPlugin(IPlugin* plugin);
  }

  /**
   * @brief Plugin class for integrating SFML window functionality.
   */
  class HC_WINDOW_SFML_EXPORT WindowSfmlPlugin : public IWindowManagerPlugin
  {
  public:
    WindowSfmlPlugin();
    virtual ~WindowSfmlPlugin();

    /**
     * @copydoc IPlugin::onConnect
     */
    void onConnect() override;

    /**
     * @copydoc IPlugin::onClose
     */
    void onClose() override;

    /**
     * @copydoc IWindowManagerPlugin::createWindowManager
     */
    IWindowManager& getWindowManager() override;

  private:
    SfmlWindowManager m_windowManager;
  };
}
