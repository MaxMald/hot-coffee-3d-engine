#pragma once

#include "hc/hcIPlugin.h"

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
     * @brief Creates an instance of the window manager provided by this plugin.
     *
     * @return Unique pointer to the window manager instance.
     */
    virtual UniquePtr<IWindowManager> createWindowManager() const = 0;

  protected:
    IWindowManagerPlugin() = default;
  };
}
