#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IGraphicsManager;
  class IWindow;
  class IAssetManager;
  class PluginManager;

  /**
   * @brief Accessor for the graphics manager plugin. This class is responsible for
   * creating and retrieving the graphics manager instance from the plugin manager.
   */
  struct GraphicsManagerPluginAccessor
  {
  public:
    /**
     * @brief Creates and retrieves the graphics manager instance from the plugin
     * manager.
     *
     * @param pluginManager Reference to the plugin manager to retrieve the graphics
     * plugin from.
     * @param window Reference to the window to be used for graphics manager creation.
     * @param assetManager Reference to the asset manager to be used for graphics manager
     * creation.
     *
     * @return Reference to the created graphics manager instance.
     */
    static IGraphicsManager& CreateAndGet(
      PluginManager& pluginManager,
      IWindow& window,
      IAssetManager& assetManager
    );
  };
}
