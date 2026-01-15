#pragma once

#include "hc/hcWindowSfmlPrerequisites.h"
#include "hc/hcIPlugin.h"

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
  class HC_WINDOW_SFML_EXPORT WindowSfmlPlugin : public IPlugin
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
     * @copydoc IPlugin::getData
     */
    void* getData() override;

    /**
     * @copydoc IPlugin::addDependencies
     */
    void addDependencies(DependencyContainer& container) override;
  };
}
