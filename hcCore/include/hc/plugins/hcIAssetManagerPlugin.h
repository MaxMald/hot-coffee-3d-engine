#pragma once

#include "hc/plugins/hcIPlugin.h"

namespace hc
{
  class IAssetManager;

  /**
   * @brief Interface for asset manager plugins.
   */
  class HC_CORE_EXPORT IAssetManagerPlugin : public IPlugin
  {
  public:
    virtual ~IAssetManagerPlugin() = default;

    /**
     * Gets the instance of the asset manager provided by this plugin.
     *
     * @return A reference to the asset manager instance.
     */
    virtual IAssetManager& getAssetManager() = 0;

  protected:
    IAssetManagerPlugin() = default;
  };
}
