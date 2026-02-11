#include "hc/hcAssetManagerLoadersRegistry.h"
#include "hc/hcAssetManager.h"
#include "hc/hcPluginManager.h"
#include "hc/hcIPlugin.h"
#include "hc/hcPluginStandardKeys.h"
#include "hc/hcMaterialDescriptor.h"
#include "hc/hcJsonDeserializerAssetLoader.h"
#include "hc/hcIAssetLoaderPlugin.h"

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
