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
     * Creates an instance of the asset manager provided by this plugin.
     *
     * @return A unique pointer to the created asset manager instance.
     */
    virtual UniquePtr<IAssetManager> createAssetManager() const = 0;

  protected:
    IAssetManagerPlugin() = default;
  };
}
