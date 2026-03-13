#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IWindowManager;
  class PluginManager;

  /**
   * Provides access to the window manager plugin and its services.
   */
  struct WindowManagerPluginAccessor
  {
    /**
     * Retrieves the window manager instance from the connected window manager plugin.
     *
     * @param pluginManager The plugin manager to retrieve the window manager from.
     * 
     * @return A reference to the window manager instance provided by the plugin.
     * 
     * @throws RuntimeErrorException if the window plugin is not found or does not
     * implement the IWindowManagerPlugin interface.
     */
    static IWindowManager& GetWindowManager(PluginManager& pluginManager);
  };
}
