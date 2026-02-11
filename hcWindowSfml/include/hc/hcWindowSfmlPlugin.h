#pragma once

#include <hc/hcIWindowManagerPlugin.h>
#include "hc/hcWindowSfmlPrerequisites.h"

namespace hc
{
  extern "C"
  {
    HC_WINDOW_SFML_EXPORT IPlugin* createWindowSfmlPlugin();
    HC_WINDOW_SFML_EXPORT void destroyWindowSfmlPlugin();
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
    UniquePtr<IWindowManager> createWindowManager() const override;
  };
}
