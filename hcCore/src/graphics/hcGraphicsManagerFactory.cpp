#include "hc/graphics/hcGraphicsManagerFactory.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/plugins/hcPluginStandardKeys.h"
#include "hc/plugins/hcIGraphicsManagerPlugin.h"
#include "hc/graphics/hcIGraphicsManager.h"
#include "hc/graphics/resource/material/hcMaterialFactoriesManager.h"
#include "hc/graphics/resource/material/hcMaterialFactoriesManagerRegistry.h"

namespace hc
{
  namespace graphicsManagerFactory
  {
    UniquePtr<IGraphicsManager> Create(
      PluginManager& pluginManager,
      IWindow& window,
      IAssetManager& assetManager
    )
    {
      SharedPtr<IPlugin> plugin = pluginManager.getPlugin(
        pluginStandardKeys::GraphicsPlugin
      );

      if (!plugin)
      {
        throw RuntimeErrorException(
          "Graphics plugin not found. Make sure the graphics plugin is correctly connected."
        );
      }

      SharedPtr<IGraphicsManagerPlugin> graphicsPlugin =
        std::dynamic_pointer_cast<IGraphicsManagerPlugin>(plugin);

      if (!graphicsPlugin)
      {
        throw RuntimeErrorException(
          "Failed to cast plugin to IGraphicsManagerPlugin. Make sure the graphics plugin implements IGraphicsManagerPlugin."
        );
      }

      UniquePtr<MaterialFactoriesManager> materialFactoriesManager = 
        MakeUnique<MaterialFactoriesManager>();

      materialFactoriesManagerRegistry::resigtryMaterialFactories(
        *materialFactoriesManager
      );

      UniquePtr<IGraphicsManager> graphicsManager = graphicsPlugin->createGraphicsManager(
        window,
        assetManager,
        std::move(materialFactoriesManager)
      );

      if (!graphicsManager)
      {
        throw RuntimeErrorException(
          "Failed to create graphics manager from plugin. Make sure the graphics plugin's createGraphicsManager function is correctly implemented and returns a valid IGraphicsManager instance."
        );
      }

      return graphicsManager;
    }
  }
}
