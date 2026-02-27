#include "hc/assets/hcAssetManagerFactory.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/plugins/hcPluginStandardKeys.h"
#include "hc/plugins/hcIAssetManagerPlugin.h"

namespace hc
{
  UniquePtr<IAssetManager> AssetManagerFactory::Create(
    PluginManager& pluginManager
  )
  {
    SharedPtr<IPlugin> plugin = pluginManager.getPlugin(
      pluginStandardKeys::AssetManagerPlugin
    );

    if (!plugin)
    {
      throw RuntimeErrorException(
        "Asset manager plugin not found. Make sure the asset manager plugin is correctly connected."
      );
    }

    SharedPtr<IAssetManagerPlugin> assetManagerPlugin =
      std::dynamic_pointer_cast<IAssetManagerPlugin>(plugin);

    if (!assetManagerPlugin)
    {
      throw RuntimeErrorException(
        "The connected asset manager plugin does not implement the IAssetManagerPlugin interface."
      );
    }

    UniquePtr<IAssetManager> assetManager =
      assetManagerPlugin->createAssetManager();

    if (!assetManager)
    {
      throw RuntimeErrorException(
        "Failed to create asset manager from plugin. Make sure the asset manager plugin's createAssetManager function is correctly implemented and returns a valid IAssetManager instance."
      );
    }

    return assetManager;
  }
}
