#include "hc/assets/hcAssetManagerLoadersRegistry.h"
#include "hc/assets/hcAssetManager.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/plugins/hcIPlugin.h"
#include "hc/plugins/hcPluginStandardKeys.h"
#include "hc/assets/hcMaterialDescriptor.h"
#include "hc/assets/hcJsonDeserializerAssetLoader.h"
#include "hc/plugins/hcIAssetLoaderPlugin.h"

namespace hc
{
  namespace assetManagerLoadersRegistry
  {
    void registerLoaders(
      AssetManager& assetManager,
      PluginManager& pluginManager
    )
    {
      // Core Asset Loaders

      assetManager.addLoader<MaterialDescriptor>(
        MakeUnique<JsonDeserializerAssetLoader<MaterialDescriptor>>()
      );

      // Plugin Asset Loaders

      SharedPtr<IPlugin> plugin = pluginManager.getPlugin(
        pluginStandardKeys::AssetLoadersPlugin
      );

      if (!plugin)
      {
        throw RuntimeErrorException(
          String::Format(
            "Failed to register asset loaders: Plugin '%s' not found.",
            pluginStandardKeys::AssetLoadersPlugin
          )
        );
      }

      SharedPtr<IAssetLoaderPlugin> assetLoaderPlugin =
        std::dynamic_pointer_cast<IAssetLoaderPlugin>(plugin);

      if (!assetLoaderPlugin)
      {
        throw RuntimeErrorException(
          String::Format(
            "Failed to register asset loaders: Plugin '%s' does not implement IAssetLoaderPlugin.",
            pluginStandardKeys::AssetLoadersPlugin
          )
        );
      }

      assetLoaderPlugin->registerAssetLoaders(assetManager);
    }
  }
}
