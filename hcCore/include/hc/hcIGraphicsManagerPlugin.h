#pragma once

#include "hc/hcIPlugin.h"

namespace hc
{
  class IGraphicsManager;
  class IWindow;
  class AssetManager;
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
     * @param materialFactoriesManager Unique pointer to the material factories
     * manager.
     *
     * @return Unique pointer to the created graphics manager. nullptr if
     * creation fails.
     */
    virtual UniquePtr<IGraphicsManager> createGraphicsManager(
      IWindow& window,
      AssetManager& assetManager,
      UniquePtr<MaterialFactoriesManager> materialFactoriesManager
    ) const = 0;
  };
}
