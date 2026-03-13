#include "hc/graphics/hcGraphicsManagerPluginAccessor.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/plugins/hcPluginStandardKeys.h"
#include "hc/plugins/hcIGraphicsManagerPlugin.h"
#include "hc/graphics/hcIGraphicsManager.h"

namespace hc
{
  IGraphicsManager& GraphicsManagerPluginAccessor::CreateAndGet(
    PluginManager& pluginManager,
    IWindow& window,
    IAssetManager& assetManager
  )
  {
    IPlugin& plugin = pluginManager.getPlugin(
      pluginStandardKeys::GraphicsPlugin
    );

    IGraphicsManagerPlugin* graphicsPlugin =
      static_cast<IGraphicsManagerPlugin*>(&plugin);

    bool result = graphicsPlugin->createGraphicsManager(
      window,
      assetManager
    );

    if (!result)
    {
      throw RuntimeErrorException(
        "Failed to create graphics manager from plugin. Make sure the graphics plugin's createGraphicsManager function is correctly implemented and returns true on success."
      );
    }

    return graphicsPlugin->getGraphicsManager();
  }
}
