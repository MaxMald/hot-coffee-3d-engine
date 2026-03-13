#pragma once

#include "hc/plugins/hcIPlugin.h"

namespace hc
{
  class IWindowManager;

  /**
   * @brief Interface for window manager plugins.
   */
  class HC_CORE_EXPORT IWindowManagerPlugin : public IPlugin
  {
  public:
    virtual ~IWindowManagerPlugin() = default;

    /**
     * @brief Gets the window manager instance provided by this plugin.
     *
     * @return The reference to the window manager instance.
     */
    virtual IWindowManager& getWindowManager() = 0;

  protected:
    IWindowManagerPlugin() = default;
  };
}
