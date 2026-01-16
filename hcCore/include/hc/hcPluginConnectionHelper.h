#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  struct PluginManagerSettings;
  class PluginManager;

  /**
   * @brief Helper functions for connecting plugins to the PluginManager.
   */
  namespace pluginConnectionHelper
  {
    /**
     * @brief Connects plugins to the PluginManager based on the provided
     * settings.
     * 
     * @param pluginManager Reference to the PluginManager to which plugins
     * will be connected.
     * @param settings Configuration settings that determine which plugins to
     * connect.
     */
    void connectToPluginsFromSettings(
      PluginManager& pluginManager,
      const PluginManagerSettings& settings
    );

    /**
     * @brief Connects the SFML window plugin to the given PluginManager.
     *
     * @param pluginManager Reference to the PluginManager to which the plugin
     * will be connected.
     */
    void connectWindowSfmlPlugin(PluginManager& pluginManager);

    /**
     * @brief Connects the OpenGL graphics plugin to the given PluginManager.
     * 
     * @param pluginManager Reference to the PluginManager to which the plugin
     * will be connected.
     */
    void connectGraphicsOpenGLPlugin(PluginManager& pluginManager);
  }
}
