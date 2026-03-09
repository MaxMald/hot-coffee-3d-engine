#include "hc/assets/hcAssetManagerPluginAccessor.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/plugins/hcPluginStandardKeys.h"
#include "hc/plugins/hcIAssetManagerPlugin.h"

namespace hc
{
  IAssetManager& AssetManagerPluginAccessor::GetAssetManager(
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
      std::reinterpret_pointer_cast<IAssetManagerPlugin>(plugin);

    if (!assetManagerPlugin)
    {
      throw RuntimeErrorException(
        "The connected asset manager plugin does not implement the IAssetManagerPlugin interface."
      );
    }

    return assetManagerPlugin->getAssetManager();
  }
}
