#include "hc/hcGraphicsManagerFactory.h"
#include "hc/hcPluginManager.h"
#include "hc/hcPluginStandardKeys.h"
#include "hc/hcIGraphicsManagerPlugin.h"
#include "hc/hcIGraphicsManager.h"
#include "hc/hcMaterialFactoriesManager.h"
#include "hc/hcMaterialFactoriesManagerRegistry.h"

namespace hc
{
  namespace graphicsManagerFactory
  {
    UniquePtr<IGraphicsManager> Create(
      PluginManager& pluginManager,
      IWindow& window,
      AssetManager& assetManager
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
