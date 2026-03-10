#pragma once

#include "hc/plugins/hcIPlugin.h"

namespace hc
{
  class IGraphicsManager;
  class IWindow;
  class IAssetManager;
  class MaterialFactoriesManager;

  /**
   * @brief Interface for graphics manager plugins.
   *
   * Provides a contract for plugins that create graphics manager instances.
   */
  class HC_CORE_EXPORT IGraphicsManagerPlugin : public IPlugin 
  {
  public:
    /**
     * @brief Creates a new graphics manager instance.
     *
     * @param window Reference to the window used for graphics initialization.
     * @param assetManager Reference to the asset manager for resource handling.
     *
     * @return true if the graphics manager was successfully created, false otherwise.
     */
    virtual bool createGraphicsManager(
      IWindow& window,
      IAssetManager& assetManager
    ) = 0;

    /**
     * @brief Retrieves the graphics manager instance created by this plugin.
     *
     * @return Reference to the graphics manager instance.
     */
    virtual IGraphicsManager& getGraphicsManager() = 0;
  };
}
